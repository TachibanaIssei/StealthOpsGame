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

	InitFontRender();

	InitSkyCube();

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
	m_fontRender.Draw(rc);
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetScale(400.0f);
	m_skyCube->SetLuminance(1.0f);
}

void Game::InitFontRender()
{
	wchar_t text[256];
	swprintf_s(text, L"テキスト見えるよ");
	m_fontRender.SetText(text);
	m_fontRender.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_fontRender.SetShadowParam(true, 2.0f, { 1.0f,1.0f,0.0f,1.0f });
	m_fontRender.SetPosition({ 0.0f,500.0f,0.0f });
}
