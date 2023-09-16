#include "k2EngineLowPreCompile.h"
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	void CascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
		Vector3 lightDirection, 
		float cascadeAreaRateTbl[NUM_SHADOW_MAP]
	)
	{
		float maxFar = g_camera3D->GetFar() * cascadeAreaRateTbl[NUM_SHADOW_MAP - 1];
		//ビュー行列を計算
		Matrix viewMatrix;
		Vector3 lightTarget = g_camera3D->GetPosition();
		Vector3 lightPos = lightTarget;
		//ライトの高さは50m
		float lightMaxHeight = 5000.0f;
		lightPos += (lightDirection) * (lightMaxHeight / lightDirection.y);
		if (fabsf(lightDirection.y) > 0.9999f)
		{
			
		}
	}
}