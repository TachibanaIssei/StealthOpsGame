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

	void ModelRender::Init(const char* tkmFilePath, AnimationClip* animationClips, const int numAnimationClips, const EnModelUpAxis enModelUpAxis, const bool shadow)
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

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.Draw(rc, 1);
		}
	}
}