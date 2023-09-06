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
		//モデルを描画
		FowardRendering(rc);
		Render2D(rc);
		CopyMainRenderTargetToFrameBuffer(rc);

		m_renderObjects.clear();
	}
	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲット
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

		//m_2DSpriteの初期化
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダ―ターゲット
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// 解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		m_2DSprite.Init(spriteInitData);

		//m_mainSpriteの初期化
		//テクスチャはメインレンダ―ターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度は2Dレンダ―ターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		m_mainSprite.Init(spriteInitData);
	}
	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;
		//メインレンダーターゲットのテクスチャ
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//フレームバッファの解像度と同じ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		//初期化オブジェクトを使って、スプライトを初期化する
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

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}
	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		rc.ClearRenderTargetView(m_2DRenderTarget);

		//メインレンダーターゲットのカラーバッファを描画する
		m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		//レンダリングターゲットをメインレンダリングターゲットへ変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//メインレンダーターゲットに2D描画後のスプライトを描画する
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

		// ビューポートを指定する
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