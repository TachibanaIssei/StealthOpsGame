#pragma once

namespace nsK2EngineLow
{
	class Bloom : public Noncopyable
	{
	public:
		void OnInit(RenderTarget& mainRenderTarget);
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	public:
		/// <summary>
		/// ブルームが発生するしきい値を設定
		/// </summary>
		/// <param name="value">しきい値</param>
		void SetThreshold(const float value)
		{
			m_samplingLuminanceCB.threshold = value;
		}
		/// <summary>
		/// ブルームが発生するしきい値を取得
		/// </summary>
		/// <returns></returns>
		const float GetThreshold()const
		{
			return m_samplingLuminanceCB.threshold;
		}

	private:
		/// <summary>
		/// レンダリングターゲットの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitRenderTarget(const RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出用のスプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitLuminanceSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ボケ画像を加算合成するスプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite(const RenderTarget& mainRenderTarget);
		/// <summary>
		/// ガウシアンブラーの初期化
		/// </summary>
		void InitGaussianBlur();

	private:
		struct SamplingLuminanceCB
		{
			float threshold = 1.0f;	//ブルームが発生するしきい値
		};

		RenderTarget				m_luminnceRenderTarget;	//輝度抽出用レンダリングターゲット
		Sprite						m_luminanceSprite;		//輝度抽出画像
		Sprite						m_finalSprite;			//最終画像
		std::array<GaussianBlur, 4>	m_gaussianBlur;			//ガウシアンブラー
		SamplingLuminanceCB			m_samplingLuminanceCB;	//輝度抽出パスに送る定数バッファ
	};
}