#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �����_�[���[�̊��N���X
	/// </summary>
	class IRenderer : public Noncopyable
	{
	public:
		/// <summary>
		/// �t�H���[�h�����_�[�p�X����Ă΂�鏈��
		/// </summary>
		/// <param name="rc"></param>
		virtual void  OnForwardRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// 2D�I�u�W�F�N�g����Ă΂�鏈��
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}