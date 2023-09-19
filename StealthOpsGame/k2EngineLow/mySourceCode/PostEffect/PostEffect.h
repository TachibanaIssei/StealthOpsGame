#pragma once

#include "mySourceCode/PostEffect/Bloom.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// ポストエフェクト
	/// </summary>
	class PostEffect : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダーターゲット</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダーターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		Bloom m_bloom;
	};
}