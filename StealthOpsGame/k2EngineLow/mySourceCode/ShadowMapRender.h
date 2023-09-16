#pragma once

#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	class IRenderer;
	class ShadowMapRender : public Noncopyable
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="ligNo">���C�g�ԍ�</param>
		/// <param name="lightDirection">���C�g�̕���</param>
		/// <param name="renderObjects">�`��I�u�W�F�N�g</param>
		/// <param name="sceneMaxPosition">�Q�[���V�[���̍ő���W</param>
		/// <param name="sceneMinPosition">�Q�[���V�[���̍ŏ����W</param>
		void Render(
			RenderContext& rc,
			int ligNo,
			Vector3& lightDirection,
			std::vector<IRenderer*>& renderObjects
		);

	private:
		CascadeShadowMapMatrix m_cascadeShadowMapMatrix;						//�J�X�P�[�h�V���h�E�}�b�v�̍s��������I�u�W�F�N�g
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];								//�����_�[�^�[�Q�b�g
		std::vector<IRenderer*> m_renderers;									//�V���h�E�}�b�v�ւ̃����_���[�̔z��
		float m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 0.05f, 0.3f,1.0f };	//�J�X�P�[�h�V���h�E�̋߁`�������̔䗦
	};
}