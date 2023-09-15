#pragma once
class Player : public IGameObject
{
public:
	enum EnAnimationClips
	{
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};

	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	const Vector3 GetPosition()const
	{
		return m_position;
	}
	const Vector3 GetScale()const
	{
		return m_scale;
	}
	const Quaternion GetRotation()const
	{
		return m_rotation;
	}

private:
	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_unityChan;
	ModelRender m_unityChanGBuffer;
	Vector3 m_position = Vector3(0.0f, 0.0f, 50.0f);
	Vector3 m_position2 = Vector3(-100.0f,0.0f,0.0f);
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

	SpriteRender m_sprite;
};

