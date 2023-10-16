#include "stdafx.h"
#include "DebugCamera.h"

void DebugCamera::Init()
{
	m_position.Set(g_camera3D->GetPosition());
	m_target.Set(g_camera3D->GetTarget());
	Vector3 toPos = m_target - m_position;
	m_toCameraPos.Set(toPos);

	m_springCamera.Init(
		g_camera3D[0],
		1000.0f,
		true,
		1.0f
	);
}

void DebugCamera::Update()
{
	Move();
	Rotation();

	m_springCamera.SetPosition(m_position);
	m_springCamera.SetTarget(m_target);
	m_springCamera.Update();
}

void DebugCamera::Move()
{
	//�E�X�e�B�b�N�̓���
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D[0].GetForward();
	Vector3 right = g_camera3D[0].GetRight();
	forward.y = 0.0f;
	right.y = 0.0f;

	forward *= y * 5.0f;
	right *= x * 5.0f;

	Vector3 moveSpeed = forward + right;

	m_position += moveSpeed;
	m_target += moveSpeed;
	m_target.y = 50.0f;

	m_toCameraPos = m_position - m_target;

	m_springCamera.SetPosition(m_position);
	m_springCamera.SetTarget(m_target);
}

void DebugCamera::Rotation()
{
	//�E�X�e�B�b�N�̓���
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Vector3 toCameraPosOld = m_toCameraPos;

	//Y����]
	Quaternion rotation;
	rotation.SetRotationDegY(1.3 * x);
	rotation.Apply(m_toCameraPos);

	//X����]
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	rotation.SetRotationDeg(axisX, 1.5f * y);
	rotation.Apply(m_toCameraPos);

	//�J�����̉�]������`�F�b�N����
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f)
	{
		//���������
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f)
	{
		//����������
		m_toCameraPos = toCameraPosOld;
	}

	m_position = m_target + m_toCameraPos;
}
