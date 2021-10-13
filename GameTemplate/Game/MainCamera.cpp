#include "stdafx.h"
#include "MainCamera.h"
#include "Player.h"

bool CMainCamera::Start() {

	g_camera3D->SetPosition(m_pos);
	g_camera3D->SetTarget(m_tar);

	return true;
}

void CMainCamera::Update() {

	//m_player = FindGO<Player>("player");
	//if (m_player != nullptr) {
	//	m_tar = m_player->GetPosition();
	//	m_tar.y += 50.0f;
	//}

	//if (g_pad[0]->IsPress(enButtonRight)) {
	//	m_pos.x -= 2.0f;
	//}
	//else if (g_pad[0]->IsPress(enButtonLeft)) {
	//	m_pos.x += 2.0f;
	//}

	////g_camera3D->SetPosition(m_pos);
	//m_pos = m_tar + toPos;

	g_camera3D->SetTarget(m_tar);
	g_camera3D->SetPosition(m_pos);
	g_camera3D->Update();

}