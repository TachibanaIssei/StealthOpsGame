﻿#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow
{
	ModelRender::ModelRender()
	{
	}
	ModelRender::~ModelRender()
	{
	}

	void ModelRender::Init(
		const char* tkmFilePath, 
		AnimationClip* animationClips,
		const int numAnimationClips, 
		const EnModelUpAxis enModelUpAxis, 
		const bool shadow)
	{
		// スケルトンを初期化
		InitSkeleton(tkmFilePath);
		// アニメーションを初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		// GBuffer描画用のモデルを初期化。
		InitModelOnRenderGBuffer(*g_renderingEngine, tkmFilePath, enModelUpAxis);
		// 各種ワールド行列を更新する
		UpdateWorldMatrixInModels();
	}

	void ModelRender::InitForwardRendering(
		const char* tkmFilePath,
		AnimationClip* animationClips = nullptr,
		const int numAnimationClips = 0,
		const EnModelUpAxis enModelUpAxis = enModelUpAxisZ)
	{
		ModelInitData modelInitData;

		//tkmファイルパスを設定
		modelInitData.m_tkmFilePath = tkmFilePath;
		//シェーダーファイルパスを設定
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//モデルの上方向を設定
		modelInitData.m_modelUpAxis = enModelUpAxis;

		// スケルトンを初期化。
		InitSkeleton(tkmFilePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//アニメーションが設定されていたら。
		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
			//スキンがある用の頂点シェーダーを設定する。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		m_forwardRenderModel.Init(modelInitData);
		// 各種ワールド行列を更新する
		UpdateWorldMatrixInModels();
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModels();

		//スケルトンが初期化されていたら
		if (m_skeleton.IsInited())
		{
			//スケルトンを更新する
			m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
		}

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::UpdateWorldMatrixInModels()
	{
		if (m_forwardRenderModel.IsInited())
		{
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, const int numAnimationClips, const EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		if (m_animationClips != nullptr)
		{
			//アニメーションの初期化
			m_animation.Init(
				m_skeleton,
				m_animationClips,
				numAnimationClips
			);
		}
	}

	void ModelRender::InitModelOnRenderGBuffer(
		const char* tkmFilePath,
		const EnModelUpAxis enModelUpAxis)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/preProcess/RenderToGBufferFor3DModel.fx";

		if (m_animationClips != nullptr) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		if (m_animationClips != nullptr) {
			modelInitData.m_skeleton = &m_skeleton;
		}
		modelInitData.m_modelUpAxis = enModelUpAxis;

		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::OnRenderToGBuffer(RenderContext& rc)
	{
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.Draw(rc, 1);
		}
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.Draw(rc, 1);
		}
	}
}