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

	public:
		/// <summary>
		/// ブルーム発生するしきい値を設定
		/// </summary>
		/// <param name="thresHold">しきい値</param>
		void SetBloomThreshold(const float thresHold)
		{
			m_bloom.SetThreshold(thresHold);
		}

	private:
		Bloom m_bloom;
	};
}