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
		/// ���C�������_�[�^�[�Q�b�g�̏�����
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// 2D�`��p�̃����_�[�^�[�Q�b�g�̏�����
		/// </summary>
		void Init2DRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e���t���[���o�b�t�@�ɃR�s�[����X�v���C�g������������
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// �t�H���[�h�����_�[���f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);
		/// <summary>
		/// 2D�`��
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// ���C�������_�[�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

	private:
		RenderTarget				m_mainRenderTarget;			//���C�������_�[�^�[�Q�b�g
		RenderTarget				m_2DRenderTarget;			//2D�`��p�̃����_�[�^�[�Q�b�g
		Sprite						m_copyToFrameBufferSprite;	//�t���[���o�b�t�@�ɃR�s�[����摜
		Sprite						m_2DSprite;					//2D�`��p�̃X�v���C�g
		Sprite						m_mainSprite;
		std::vector<IRenderer*>		m_renderObjects;			//�`��I�u�W�F�N�g�̃��X�g
	};
}
