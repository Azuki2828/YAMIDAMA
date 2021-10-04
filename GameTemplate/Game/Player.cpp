#include "stdafx.h"
#include "Player.h"

Player* g_pCurrentPlayer = nullptr;

void Normal() {
	g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Walk);
}

void Yoi() {
	g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Idle);
}

void Death() {
	g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Death);
}

int GetYoiParam() {

	return g_pCurrentPlayer->GetYoiParam();
}

void ChangeState(int state) {

	g_pCurrentPlayer->SetState(state);
}

void Move() {
	g_pCurrentPlayer->Move();
}

void Drink() {

	if (g_pad[0]->IsTrigger(enButtonA)) {
		int yoiParam = g_pCurrentPlayer->GetYoiParam();
		yoiParam += 10;
		g_pCurrentPlayer->SetYoiParam(yoiParam);
	}
}

PYBIND11_MODULE(Game, m) {
	m.def("Normal", &Normal);
	m.def("Yoi", &Yoi);
	m.def("Death", &Death);
	m.def("Move", &Move);
	m.def("Drink", &Drink);
	m.def("GetYoiParam", &GetYoiParam);
	m.def("ChangeState", &ChangeState);
}

bool Player::Start() {

	m_modelRender = NewGO<NMyRenderer::ModelRender>(0);
	m_modelRender->SetFilePathTkm("Assets/modelData/unityChan.tkm");
	m_modelRender->SetFilePathTks("Assets/modelData/unityChan.tks");
	m_animationClip[enAnim_Walk].Load("Assets/animData/walk.tka");
	m_animationClip[enAnim_Idle].Load("Assets/animData/idle.tka");
	m_animationClip[enAnim_Death].Load("Assets/animData/KneelDown.tka");
	m_animationClip[enAnim_Walk].SetLoopFlag(true);
	m_animationClip[enAnim_Idle].SetLoopFlag(true);
	m_animationClip[enAnim_Death].SetLoopFlag(false);
	m_modelRender->InitAnimation(m_animationClip, enAnimNum);
	m_modelRender->SetAnimFlg(true);
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);
	m_modelRender->SetModelUpAxis(EnModelUpAxis::enModelUpAxisY);
	m_modelRender->Init();

	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->Init(L"酔い度：", { -550.0f,310.0f });	//場所
	m_fontRender->SetColor({1.0f,0.0f,0.0f,1.0f});			//赤色
	m_fontRender->SetShadowParam(true, 1.0f, Vector4::Black);
	return true;
}

void Player::Update() {

	g_pCurrentPlayer = this;

	switch (m_playerState) {
	case EnPlayerState::enState_Normal:

		ImportModule("PlayerNormal");
		break;
	case EnPlayerState::enState_Yoi:

		ImportModule("PlayerYoi");
		break;
	case EnPlayerState::enState_Death:

		ImportModule("PlayerDeath");
		break;
	}
	auto updateFunc = m_playerPyModule.attr("Update");
	updateFunc();

	//ライトカメラを更新。
	LightCameraUpdate();

	//フォントを更新。
	FontUpdate();
}

void Player::LightCameraUpdate() {

	//ライトカメラの情報を更新
	Vector3 m_lightCameraTar = m_pos;
	Vector3 m_lightCameraPos = m_lightCameraTar;

	//ライトカメラの座標を設定
	m_lightCameraPos.y += 300.0f;
	m_lightCameraPos.z += 300.0f;

	//ライトカメラの情報を設定
	Camera::GetLightCamera()->SetPosition(m_lightCameraPos);
	Camera::GetLightCamera()->SetTarget(m_lightCameraTar);
	Camera::GetLightCamera()->SetFar(800.0f);

	//カメラの上方向を求める。
	Vector3 Vec_x = Vector3::AxisX;
	Vector3 TarPos = Camera::GetLightCamera()->GetTarget() - Camera::GetLightCamera()->GetPosition();
	TarPos.Normalize();
	Vector3 CameraUp;

	CameraUp.Cross(TarPos, Vec_x);
	CameraUp.Normalize();
	Camera::GetLightCamera()->SetUp(CameraUp);
	Camera::GetLightCamera()->Update();
}

void Player::Move() {

	if (g_pad[0]->IsPress(enButtonRight)) {
		m_pos.x -= 2.0f;
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		m_pos.x += 2.0f;
	}

	m_modelRender->SetPosition(m_pos);
}

void Player::FontUpdate() {

	wchar_t time[64];
	swprintf_s(time, L"酔い度：%d", m_yoiParam);
	m_fontRender->SetText(time);
}