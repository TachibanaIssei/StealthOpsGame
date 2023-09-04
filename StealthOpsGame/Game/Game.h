#pragma once

class Game : public IGameObject
{
public:
	enum EnAnimationClips
	{
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};
	Game();
	~Game();

	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

private:
	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_unityChan;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
};

