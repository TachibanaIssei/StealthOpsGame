#pragma once
#include "../k2EngineLow/mySourceCode/Camera/SpringCamera.h"

class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};
	bool Start() override final;
	void Update() override final;

private:
	SpringCamera m_springCamera;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_target = Vector3::Zero;
	Vector3 m_toCameraPos = Vector3::Zero;
};

