﻿#pragma once
namespace nsK2EngineLow
{
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// モデルの初期化
		/// </summary>
		/// <param name="tkmFilePath">tkmファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowReceiver">trueなら影が落ちる</param>
		void Init(
			const char* tkmFilePath,
			AnimationClip* animationClips = nullptr,
			const int numAnimationClips = 0,
			const EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadowReceiver = true
		);

		/// <summary>
		/// 特殊なシェーディングを行いたい場合の初期化
		/// </summary>
		/// <param name="initData">モデルデータ</param>
		void InitForwardRendering(
			const char* tkmFilePath,
			AnimationClip* animationClips = nullptr,
			const int numAnimationClips = 0,
			const EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadowReceiver = true
		);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// フォワードレンダーで描画されるモデルを取得する
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetFowardRenderModel()
		{
			return m_forwardRenderModel;
		}

		/// <summary>
		/// 座標、回転、拡大をすべて設定
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rotation">回転</param>
		/// <param name="scale">拡大</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="position">Vector3の座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		};

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetPosition(const float x, const float y, const float z)
		{
			SetPosition({ x, y, z });
		};

		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetScale(const float x, const float y, const float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// ボーンの名前からボーン番号を検索
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns>ボーン番号。見つからなかった場合は-1が返る</returns>
		const int FindBoneID(const wchar_t* boneName)const
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// ボーン番号からボーンを取得する
		/// </summary>
		/// <param name="boneNo">ボーン番号</param>
		/// <returns>ボーン</returns>
		const Bone* GetBone(const int boneNo) const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// アニメーションイベントを追加する
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(const AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(const int animNo, const float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		const bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

	private:
		/// <summary>
		/// 各種モデルのワールド行列を更新する
		/// </summary>
		void UpdateWorldMatrixInModels();
		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上向き。</param>
		void InitAnimation(
			AnimationClip* animationClips,
			const int numAnimationClips,
			const EnModelUpAxis enModelUpAxis
		);
		/// <summary>
		/// GBuffer描画用のモデルを初期化
		/// </summary>
		/// <param name="renderingEngine">レンダリングエンジン</param>
		/// <param name="tkmFilePath">tkmファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModelOnRenderGBuffer(
			RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			const EnModelUpAxis enModelUpAxis
		);
		/// <summary>
		/// GBuffer描画パスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderToGBuffer(RenderContext& rc) override;
		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		void OnForwardRender(RenderContext& rc) override;

	private:
		Skeleton					m_skeleton;									//スケルトン
		AnimationClip*				m_animationClips = nullptr;					//アニメーションクリップ。
		int							m_numAnimationClips = 0;					//アニメーションクリップの数。
		Animation					m_animation;								//アニメーション。
		float						m_animationSpeed = 1.0f;					//アニメーションスピード
		Vector3						m_position = Vector3::Zero;					//座標
		Vector3						m_scale = Vector3::One;						//大きさ
		Quaternion					m_rotation = Quaternion::Identity;			//回転
		Model						m_forwardRenderModel;						//フォワードレンダリングで描画されるモデル
		Model						m_renderToGBufferModel;						//RenderToGBufferで描画されるモデル
	};
}