#pragma once
class BackGround : public IGameObject
{
public:
	BackGround() {};
	~BackGround() {};
	
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext&rc) override final;

private:
	ModelRender m_backGround;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
};

