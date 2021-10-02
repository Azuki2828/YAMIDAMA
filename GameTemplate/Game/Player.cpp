#include "stdafx.h"
#include "Player.h"

bool Player::Start() {

	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->SetFilePathTkm("Assets/modelData/unityChan.tkm");
	m_modelRender->SetFilePathTks("Assets/modelData/unityChan.tks");
	m_animationClip.Load("Assets/animData/idle.tka");
	m_animationClip.SetLoopFlag(true);
	m_modelRender->InitAnimation(&m_animationClip, 1);
	m_modelRender->SetAnimFlg(true);
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetModelUpAxis(EnModelUpAxis::enModelUpAxisY);
	m_modelRender->Init();
	return true;
}

void Player::Update() {

	m_modelRender->PlayAnimation(0);

	if (g_pad[0]->IsPress(enButtonRight)) {
		m_pos.x -= 2.0f;
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		m_pos.x += 2.0f;
	}

	m_modelRender->SetPosition(m_pos);
	//ライトカメラの情報を更新
	Vector3 m_lightCameraTar = m_pos;
	Vector3 m_lightCameraPos = m_lightCameraTar;

	//ライトカメラの座標を設定
	m_lightCameraPos.y += 300.0f;

	//ライトカメラの情報を設定
	Camera::GetLightCamera()->SetPosition(m_lightCameraPos);
	Camera::GetLightCamera()->SetTarget(m_lightCameraTar);
	Camera::GetLightCamera()->SetFar(800.0f);
}