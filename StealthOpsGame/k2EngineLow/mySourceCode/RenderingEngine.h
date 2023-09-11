#pragma once

#include "MyRenderer.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// レンダリングエンジン
	/// </summary>
	class RenderingEngine : public Noncopyable
	{
	public:
		struct SDeferredLightingCB
		{
			Light m_light;	//ライト
		};

	public:
		RenderingEngine();
		~RenderingEngine();
		/// <summary>
		/// 初期化処理
		/// </summary>
		void Init();
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
		/// <summary>
		/// ディレクションライトのパラメータを設定
		/// </summary>
		/// <param name="lightNo"></param>
		/// <param name="direction"></param>
		/// <param name="color"></param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// 環境光を設定。
		/// </summary>
		/// <param name="ambient"></param>
		void SetAmbient(const Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

	private:
		/// <summary>
		/// G-Bufferを初期化
		/// </summary>
		void InitGBuffer();
		/// <summary>
		/// ディファードライティングに関連する初期化
		/// </summary>
		void InitDeferredLighting();
		/// <summary>
		/// ディファードライティングで使用する画像の初期化
		/// </summary>
		void InitDeferredLightingSprite();
		/// <summary>
		/// メインレンダーターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// 2D描画用のレンダーターゲットの初期化
		/// </summary>
		void Init2DRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容をフレームバッファにコピーするスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// G-Bufferへの描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToGBuffer(RenderContext& rc);
		/// <summary>
		/// ディファードライティング
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);
		/// <summary>
		/// フォワードレンダーモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);
		/// <summary>
		/// 2D描画
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// メインレンダーターゲットの内容をフレームバッファにコピー
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // アルベドと深度値。αに深度値が記憶されています。
			enGBufferNormal,                // 法線
			enGBufferMetaricShadowSmooth,   // メタリック、影パラメータ、スムース。
											// メタリックがr、影パラメータがg、スムースがa。gは未使用。
			enGBufferNum,                   // G-Bufferの数
		};

		SDeferredLightingCB			m_deferredLightingCB;       // ディファードライティング用の定数バッファ
		SceneLight					m_sceneLight;               // シーンライト
		RenderTarget				m_mainRenderTarget;			//メインレンダーターゲット
		RenderTarget				m_2DRenderTarget;			//2D描画用のレンダーターゲット
		RenderTarget				m_gBuffer[enGBufferNum];	//G-Bufferレンダーターゲット配列
		Sprite						m_copyToFrameBufferSprite;	//フレームバッファにコピーする画像
		Sprite						m_2DSprite;					//2D描画用のスプライト
		Sprite						m_mainSprite;
		Sprite						m_diferredLightingSprite;	//ディファードライティングを行うためのスプライト
		std::vector<IRenderer*>		m_renderObjects;			//描画オブジェクトのリスト
	};
}
