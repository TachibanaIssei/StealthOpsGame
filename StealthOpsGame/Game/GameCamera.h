#pragma once
#include "../k2EngineLow/mySourceCode/Camera/SpringCamera.h"
#include "DebugCamera.h"

class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};
	bool Start() override final;
	void Update() override final;

private:
	void Move();
	void Rotation();

private:
	DebugCamera m_debugCamera;
};

