#pragma once

#include "MyRenderer.h"
#include "SceneLight.h"
#include "ShadowMapRender.h"
#include "PostEffect/PostEffect.h"

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
			Matrix mlvp[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//ライトビュープロジェクション行列
		};

	public:
		RenderingEngine();
		~RenderingEngine();
		/// <summary>
		/// 初期化処理
		/// </summary>
		void Init(const bool isSoftShadow);
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
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライト方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// キャストシャドウフラグを取得
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <returns>trueだったら影を落とす</returns>
		void SetDirectionLightCastShadow(const int lightNo, const bool flag)
		{
			m_sceneLight.SetDirectionLightCastShadow(lightNo, flag);
		}
		/// <summary>
		/// 環境光を設定。
		/// </summary>
		/// <param name="ambient">環境光の色</param>
		void SetAmbient(const Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}
		/// <summary>
		/// ディファードライティングの定数バッファを取得
		/// </summary>
		/// <returns>定数バッファ</returns>
		SDeferredLightingCB& GetDeferredLightingCB()
		{
			return m_deferredLightingCB;
		}
		/// <summary>
		/// シャドウマップテクスチャにクエリを行う
		/// </summary>
		/// <param name="queryFunc"></param>
		void QueryShadowMapTexture(std::function< void(Texture& shadowMap) > queryFunc)
		{
			for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; i++)
			{
				for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
				{
					queryFunc(m_shadowMapRenders[i].GetShadowMap(areaNo));
				}
			}
		}

		/// <summary>
		/// ソフトシャドウをtrueなら行う
		/// </summary>
		/// <returns></returns>
		const bool IsSoftShadow() const
		{
			return m_isSoftShadow;
		}

		/// <summary>
		/// メインレンダリングターゲットのディスクリプタと
		/// ZPrepassのデプスステンシルバッファでレンダリングターゲットを設定
		/// </summary>
		/// <param name="rc"></param>
		void SetMainRenderTargetAndDepthStencilBuffer(RenderContext& rc)
		{
			rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_zPrepassRenderTarget.GetDSVCpuDescriptorHandle());
		}

	private:
		/// <summary>
		/// ZPrepassレンダーターゲットを初期化
		/// </summary>
		void InitZPrepassRenderTarget();
		/// <summary>
		/// シャドウマップレンダーを初期化
		/// </summary>
		void InitShadowMapRender();
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
		/// ZPrepassへの描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToZPrepass(RenderContext& rc);
		/// <summary>
		/// シャドウマップへの描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(RenderContext& rc);
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

		SDeferredLightingCB			m_deferredLightingCB;							// ディファードライティング用の定数バッファ
		SceneLight					m_sceneLight;									// シーンライト
		ShadowMapRender				m_shadowMapRenders[MAX_DIRECTIONAL_LIGHT];		//シャドウマップへの描画処理
		PostEffect					m_postEffect;									//ポストエフェクト
		RenderTarget				m_zPrepassRenderTarget;							//ZPrepass描画用のレンダリングターゲット
		RenderTarget				m_mainRenderTarget;								//メインレンダーターゲット
		RenderTarget				m_2DRenderTarget;								//2D描画用のレンダーターゲット
		RenderTarget				m_gBuffer[enGBufferNum];						//G-Bufferレンダーターゲット配列
		Sprite						m_copyToFrameBufferSprite;						//フレームバッファにコピーする画像
		Sprite						m_2DSprite;										//2D描画用のスプライト
		Sprite						m_mainSprite;
		Sprite						m_diferredLightingSprite;						//ディファードライティングを行うためのスプライト
		std::vector<IRenderer*>		m_renderObjects;								//描画オブジェクトのリスト

		bool						m_isSoftShadow = false;							//ソフトシャドウを行う？
	};
}
