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
		DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];
	};

	class SceneLight
	{
	};
}