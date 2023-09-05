#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		Init();
	}
	RenderingEngine::~RenderingEngine()
	{
	}
	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		Init2DRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
	}
	void RenderingEngine::Execute(RenderContext& rc)
	{
		//���f����`��
		FowardRendering(rc);
		Render2D(rc);
		CopyMainRenderTargetToFrameBuffer(rc);

		m_renderObjects.clear();
	}
	void RenderingEngine::InitMainRenderTarget()
	{
		//���C�������_�����O�^�[�Q�b�g
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}
	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//m_2DSprite�̏�����
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�\�^�[�Q�b�g
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		m_2DSprite.Init(spriteInitData);

		//m_mainSprite�̏�����
		//�e�N�X�`���̓��C�������_�\�^�[�Q�b�g
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//�𑜓x��2D�����_�\�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		m_mainSprite.Init(spriteInitData);
	}
	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;
		//���C�������_�[�^�[�Q�b�g�̃e�N�X�`��
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//�t���[���o�b�t�@�̉𑜓x�Ɠ���
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		//�������I�u�W�F�N�g���g���āA�X�v���C�g������������
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}
	void RenderingEngine::FowardRendering(RenderContext& rc)
	{
		BeginGPUEvent("FowardRendering");

		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnForwardRender(rc);
		}

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}
	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		rc.ClearRenderTargetView(m_2DRenderTarget);

		//���C�������_�[�^�[�Q�b�g�̃J���[�o�b�t�@��`�悷��
		m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�֕ύX����
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//���C�������_�[�^�[�Q�b�g��2D�`���̃X�v���C�g��`�悷��
		m_2DSprite.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}
	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyToFrameBufferSprite.Draw(rc);

		EndGPUEvent();
	}
}