#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	void ShadowMapRender::Init()
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

		DXGI_FORMAT colorFormat;
		DXGI_FORMAT depthFormat;
		colorFormat = g_hardShadowMapFormat.colorBufferFormat;
		depthFormat = g_hardShadowMapFormat.depthBufferFormat;

		//近景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_NEAR].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat
		);
		//中景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_MIDDLE].Create(
			1024,
			1024,
			1,
			1,
			colorFormat,
			depthFormat
		);
		//遠景用のシャドウマップ
		m_shadowMaps[SHADOW_MAP_AREA_FAR].Create(
			512,
			512,
			1,
			1,
			colorFormat,
			depthFormat
		);
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
	}
}