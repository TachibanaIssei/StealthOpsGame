#include "stdafx.h"
#include "GameCamera.h"

bool GameCamera::Start()
{
	m_debugCamera.Init();

	return true;
}

void GameCamera::Update()
{
	m_debugCamera.Update();
}

void GameCamera::Move()
{
	
}

void GameCamera::Rotation()
{
	
}
