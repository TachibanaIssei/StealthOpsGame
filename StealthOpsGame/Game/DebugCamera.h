#pragma once
#include "../k2EngineLow/mySourceCode/Camera/SpringCamera.h"

class DebugCamera
{
public:
	void Init();
	void Update();

private:
	void Move();
	void Rotation();

private:
	SpringCamera m_springCamera;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_target = Vector3::Zero;
	Vector3 m_toCameraPos = Vector3::Zero;
};

