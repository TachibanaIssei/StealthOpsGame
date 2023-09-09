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

	struct Light
	{
		DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];	//ディレクショナルライトの配列
		Vector3 eyePos;			//カメラの位置
		float pad0;				//パディング0
		Vector3 ambientLight;	//環境光
		float pad1;				//パディング1
	};

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
		Light& GetSeneLight()
		{
			return m_light;
		}
		void SetDirectionLight(int lightNo, Vector3 direction, Vector4 color)
		{
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color		= color;
		}
		bool IsCastShadow(int lightNo)
		{
			return m_light.directionalLight[lightNo].castShadow;
		}
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

	private:
		Light m_light;  //シーンライト
	};
}