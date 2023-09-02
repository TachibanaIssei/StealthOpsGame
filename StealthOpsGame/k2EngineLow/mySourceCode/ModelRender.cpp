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

		//tkm�t�@�C���p�X��ݒ�
		modelInitData.m_tkmFilePath = tkmFilePath;
		//�V�F�[�_�[�t�@�C���p�X��ݒ�
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//���f���̏������ݒ�
		modelInitData.m_modelUpAxis = enModelUpAxis;

		// �X�P���g�����������B
		InitSkeleton(tkmFilePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//�A�j���[�V�������ݒ肳��Ă�����B
		if (m_animationClips != nullptr)
		{
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L��������p�̒��_�V�F�[�_�[��ݒ肷��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		m_forwardRenderModel.Init(modelInitData);
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModels();

		//�X�P���g��������������Ă�����
		if (m_skeleton.IsInited())
		{
			//�X�P���g�����X�V����
			m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
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
		//�X�P���g���̃f�[�^��ǂݍ��݁B
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
			//�A�j���[�V�����̏�����
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