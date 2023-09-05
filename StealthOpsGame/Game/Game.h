#pragma once
#include "Player.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();

	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

private:
	Player* m_player;
	SpriteRender m_spriteRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
};

