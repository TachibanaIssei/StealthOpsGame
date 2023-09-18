#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
    m_backGround.Init("Assets/modelData/ground.tkm",nullptr,0,enModelUpAxisZ);
    m_backGround.SetPosition(m_position);
    m_backGround.SetScale(m_scale);
    m_backGround.SetRotation(m_rotation);

    m_backGround.Update();
    return true;
}

void BackGround::Update()
{
    m_backGround.Update();
}

void BackGround::Render(RenderContext& rc)
{
    m_backGround.Draw(rc);
}
