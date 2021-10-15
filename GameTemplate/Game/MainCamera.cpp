#include "stdafx.h"
#include "MainCamera.h"
#include "player/Player.h"

namespace nsMyGame {

	bool CMainCamera::Start() {

		g_camera3D->SetPosition(m_pos);
		g_camera3D->SetTarget(m_tar);

		return true;
	}

	void CMainCamera::Update() {

		m_player = FindGO<nsPlayer::CPlayer>("player");
		if (m_player != nullptr) {
			m_tar = m_player->GetPosition();
		}

		m_pos = m_tar + toPos;

		g_camera3D->SetTarget(m_tar);
		g_camera3D->SetPosition(m_pos);
		g_camera3D->Update();

	}
}