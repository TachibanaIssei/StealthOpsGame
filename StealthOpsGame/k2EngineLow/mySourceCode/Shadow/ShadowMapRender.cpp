﻿#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	void ShadowMapRender::Init(const bool isSoftShadow)
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

		DXGI_FORMAT colorFormat;
		DXGI_FORMAT depthFormat;

		if (isSoftShadow)
		{
			colorFormat = g_softShadowMapFormat.colorBufferFormat;
			depthFormat = g_softShadowMapFormat.depthBufferFormat;
		}
		else
		{
			colorFormat = g_hardShadowMapFormat.colorBufferFormat;
			depthFormat = g_hardShadowMapFormat.depthBufferFormat;
		}

		//近景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_NEAR].Create(
			4096,
			4096,
			1,
			1,
			colorFormat,
			depthFormat
		);
		//中景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_MIDDLE].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat
		);
		//遠景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_FAR].Create(
			1024,
			1024,
			1,
			1,
			colorFormat,
			depthFormat
		);

		if (isSoftShadow)
		{
			m_gausBlur[0].Init(&m_shadowMaps[0].GetRenderTargetTexture());
			m_gausBlur[1].Init(&m_shadowMaps[1].GetRenderTargetTexture());
			m_gausBlur[2].Init(&m_shadowMaps[2].GetRenderTargetTexture());
		}
		m_isSoftShadow = isSoftShadow;
	}
	void ShadowMapRender::Render(
		RenderContext& rc, 
		int ligNo, 
		Vector3& lightDirection, 
		std::vector<IRenderer*>& renderObjects
	)
	{
		if (lightDirection.LengthSq() < 0.001f)
		{
			return;
		}
		m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
			lightDirection,
			m_cascadeAreaRateArray
		);

		int shadowMapNo = 0;
		for (auto& shadowMap : m_shadowMaps)
		{
			rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
			rc.SetRenderTargetAndViewport(shadowMap);
			rc.ClearRenderTargetView(shadowMap);

			for (auto& renderer : renderObjects)
			{
				renderer->OnRenderShadowMap(
					rc,
					ligNo,
					shadowMapNo,
					m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
				);
			}
			rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
			shadowMapNo++;
		}

		if (m_isSoftShadow)
		{
			//ブラーを実行
			for (auto& blur : m_gausBlur)
			{
				blur.ExecuteOnGPU(rc, 1.0f);
			}
		}
	}
}