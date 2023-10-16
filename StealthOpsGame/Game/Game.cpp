#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	m_player = NewGO<Player>(0,"Player");
	m_backGround = NewGO<BackGround>(0, "BackGround");
	m_spriteRender.Init("Assets/sprite/attackUP.DDS", 256, 256);


	m_rotation.SetRotationDegZ(90.0f);
	m_spriteRender.SetRotation(m_rotation);
	m_spriteRender.Update();

	InitFontRender();

	Vector3 dir = Vector3(1.0f, -1.0f, 0.0f);
	dir.Normalize();
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	g_renderingEngine->SetDirectionLight(0, dir, color);
	Vector3 dir1 = Vector3(-1.0f, -1.0f, 0.0f);
	dir1.Normalize();
	Vector3 color1 = Vector3(1.0f, 1.0f, 1.0f);
	g_renderingEngine->SetDirectionLight(1, dir1, color1);
	g_renderingEngine->SetDirectionLightCastShadow(1, true);
	g_renderingEngine->SetAmbient({ 0.5f,0.5f,0.5f });
	InitSkyCube();

	g_renderingEngine->SetBloomThreshold(1.0f);

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
	//m_spriteRender.Draw(rc);
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
