#include "k2EngineLowPreCompile.h"
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
		InitModelOnRenderGBuffer(tkmFilePath, enModelUpAxis);
		// 各種ワールド行列を更新する
		UpdateWorldMatrixInModels();
	}

	void ModelRender::InitForwardRendering(ModelInitData initData)
	{
		InitSkeleton(initData.m_tkmFilePath);
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_forwardRenderModel.Init(initData);
		UpdateWorldMatrixInModels();
	}

	void ModelRender::InitTrancelucent(const char* tkmFilePath, AnimationClip* animationClips, const int numAnimationClips, const EnModelUpAxis enModelUpAxis)
	{
		//スケルトンを初期化
		InitSkeleton(tkmFilePath);
		//アニメーションを初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		//半透明モデルを初期化
		InitModelOnTranslucent(tkmFilePath, enModelUpAxis);
		//各種ワールド行列を更新
		UpdateWorldMatrixInModels();
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModels();

		//スケルトンが初期化されていたら
		if (m_skeleton.IsInited())
		{
			//スケルトンを更新する
			if (m_renderToGBufferModel.IsInited())
			{
				m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
			}
			else if (m_translucentModel.IsInited())
			{
				m_skeleton.Update(m_translucentModel.GetWorldMatrix());
			}
		}

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		modelInitData.m_vsSkinEntryPointFunc = "VSMain";
		modelInitData.m_vsEntryPointFunc = "VSMain";

		if (m_animationClips != nullptr) {
			// アニメーションあり。
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}
	}

	void ModelRender::UpdateWorldMatrixInModels()
	{
		if (m_renderToGBufferModel.IsInited())
		{
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_forwardRenderModel.IsInited())
		{
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_translucentModel.IsInited())
		{
			m_translucentModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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

	void ModelRender::InitModelOnTranslucent(const char* tkmFilePath, const EnModelUpAxis enModelUpAxis)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//頂点シェーダーのエントリーポイントを設定
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr) {
			modelInitData.m_skeleton = &m_skeleton;
		}
		modelInitData.m_psEntryPointFunc = "PSMainSoftShadow";

		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetDeferredLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetDeferredLightingCB());
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		modelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

		m_translucentModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnShadowMap(const char* tkmFilePath, EnModelUpAxis modelUpAxis, bool isFrontCullingOnDrawShadowMap)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		if (isFrontCullingOnDrawShadowMap)
		{
			//表カリングを行う
			modelInitData.m_cullMode = D3D12_CULL_MODE_FRONT;
		}
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr)
		{
			modelInitData.m_skeleton = &m_skeleton;
		}
		modelInitData.m_fxFilePath = "Assets/shader/preProcess/DrawShadowMap.fx";

		modelInitData.m_colorBufferFormat[0] = g_hardShadowMapFormat.colorBufferFormat;

		for(int ligNo = 0; ligNo < NUM_SHADOW_LIGHT; )
	}

	void ModelRender::InitModelOnRenderGBuffer(
		const char* tkmFilePath,
		const EnModelUpAxis enModelUpAxis)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/preProcess/RenderToGBufferFor3DModel.fx";

		//頂点シェーダーのエントリーポイントを設定
		SetupVertexShaderEntryPointFunc(modelInitData);
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
	void ModelRender::OnTlanslucentRender(RenderContext& rc)
	{
		if (m_translucentModel.IsInited()) {
			m_translucentModel.Draw(rc, 1);
		}
	}
}