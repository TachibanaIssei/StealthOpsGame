#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_unityChan.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);

	m_rotation.SetRotationDegY(180.0f);
	m_unityChan.SetRotation(m_rotation);
	m_unityChan.Update();

	m_sprite.Init("Assets/sprite/attackUP.DDS", 256, 256);
	return true;
}

void Player::Update()
{
	if (g_pad[0]->IsTrigger(enButtonRB2))
	{
		m_unityChan.PlayAnimation(enAnimationClip_Idle, 0.2f);
	}
	else if (g_pad[0]->IsTrigger(enButtonLB2))
	{
		m_unityChan.PlayAnimation(enAnimationClip_Walk, 0.2f);
	}
	else if (g_pad[0]->IsTrigger(enButtonLB3))
	{
		m_unityChan.PlayAnimation(enAnimationClip_Run, 0.1f);
	}
	if (g_pad[0]->IsPress(enButtonX))
	{
		m_position.x -= 0.5f;
	}
	else if (g_pad[0]->IsPress(enButtonB))
	{
		m_position.x += 0.5f;
	}
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_position.y -= 0.5f;
	}
	else if (g_pad[0]->IsPress(enButtonRB1))
	{
		m_position.z += 0.5f;
	}
	if (g_pad[0]->IsPress(enButtonLB1))
	{
		m_position.z -= 0.5f;
	}
	else if (g_pad[0]->IsPress(enButtonY))
	{
		m_position.y += 0.5f;
	}
	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_scale.x += 0.1f;
		m_scale.y += 0.1f;
		m_scale.z += 0.1f;
	}
	else if (g_pad[0]->IsPress(enButtonDown))
	{
		m_scale.x -= 0.1f;
		m_scale.y -= 0.1f;
		m_scale.z -= 0.1f;
	}
	if (g_pad[0]->IsPress(enButtonRight))
	{
		m_rotation.AddRotationDegY(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonLeft))
	{
		m_rotation.AddRotationDegY(-1.0f);
	}

	m_unityChan.SetTRS(m_position, m_rotation, m_scale);
	m_unityChan.Update();

	m_sprite.Update();
}

void Player::Render(RenderContext& rc)
{
	m_unityChan.Draw(rc);
	m_sprite.Draw(rc);
}
