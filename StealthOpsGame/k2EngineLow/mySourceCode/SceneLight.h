#pragma once

namespace nsK2EngineLow
{

	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//ライトの方向
		int castShadow;		//影をキャストする？
		Vector4 color;		//ライトのカラー
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];	//ディレクショナルライトの配列
		Matrix mViewProjInv;	//ビュープロジェクション行列の逆行列
		Vector3 eyePos;			//カメラの位置
		float pad0;				//パディング0
		Vector3 ambientLight;	//環境光
		float pad1;				//パディング1
	};

	/// <summary>
	/// シーンライトクラス
	/// </summary>
	class SceneLight : public Noncopyable
	{
	public:
		SceneLight() {};
		~SceneLight() {};

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns></returns>
		Light& GetSceneLight()
		{
			return m_light;
		}
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector4 color)
		{
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color		= color;
		}
		bool IsCastShadow(const int lightNo)
		{
			return m_light.directionalLight[lightNo].castShadow;
		}
		void SetAmbient(const Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

	private:
		Light m_light;  //シーンライト
	};
}