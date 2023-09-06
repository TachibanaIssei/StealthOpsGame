#pragma once
#include "Player.h"
#include "../k2EngineLow/mySourceCode/SkyCube.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();

	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

private:
	void InitSkyCube();
	void InitFontRender();

private:
	Player* m_player = nullptr;
	SpriteRender m_spriteRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

	FontRender m_fontRender;

	SkyCube* m_skyCube;
};

