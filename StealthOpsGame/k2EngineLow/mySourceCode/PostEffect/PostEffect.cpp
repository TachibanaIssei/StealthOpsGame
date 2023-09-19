#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		m_bloom.OnInit(mainRenderTarget);
	}
	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		BeginGPUEvent("PostEffect");

		//ブルーム
		m_bloom.OnRender(rc, mainRenderTarget);

		g_renderingEngine->SetMainRenderTargetAndDepthStencilBuffer(rc);

		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//エフェクトの描画
		EffectEngine::GetInstance()->Draw();
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		EndGPUEvent();
	}
}