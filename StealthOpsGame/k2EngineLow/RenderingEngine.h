#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// �����_�����O�G���W��
	/// </summary>
	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		/// <summary>
		/// �`��I�u�W�F�N�g��ǉ��B
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
		/// <summary>
		/// �`�揈�������s����
		/// </summary>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// ����������
		/// </summary>
		void Init();
		/// <summary>
		/// �����_�[�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// �摜�̏�����
		/// </summary>
		void InitSprites();
		/// <summary>
		/// �t�H���[�h�����_�[���f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);

	private:
		RenderTarget				m_mainRenderTarget;
		Sprite						m_copyToFrameBufferSprite;	//�t���[���o�b�t�@�ɃR�s�[����摜
		std::vector<IRenderer*>		m_renderObjects;			//�`��I�u�W�F�N�g�̃��X�g
	};
}
