#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_player = NewGO<Player>(0,"Player");
	m_spriteRender.Init("Assets/sprite/attackUP.DDS", 256, 256);


	m_rotation.SetRotationDegZ(90.0f);
	m_spriteRender.SetRotation(m_rotation);
	m_spriteRender.Update();
	return true;
}

void Game::Update()
{
	m_position = m_player->GetPosition();
	m_position.z = 0.0f;
	m_scale = m_player->GetScale();
	m_scale.z = 1.0f;

	m_spriteRender.SetPosition(m_position);
	m_spriteRender.SetScale(m_scale);
	m_spriteRender.SetRotation(m_rotation);
	m_spriteRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
