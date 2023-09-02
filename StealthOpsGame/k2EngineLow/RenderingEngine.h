#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// レンダリングエンジン
	/// </summary>
	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		/// <summary>
		/// 描画オブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
		/// <summary>
		/// 描画処理を実行する
		/// </summary>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// 初期化処理
		/// </summary>
		void Init();
		/// <summary>
		/// レンダーターゲットの初期化
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// 画像の初期化
		/// </summary>
		void InitSprites();
		/// <summary>
		/// フォワードレンダーモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);

	private:
		RenderTarget				m_mainRenderTarget;
		Sprite						m_copyToFrameBufferSprite;	//フレームバッファにコピーする画像
		std::vector<IRenderer*>		m_renderObjects;			//描画オブジェクトのリスト
	};
}
