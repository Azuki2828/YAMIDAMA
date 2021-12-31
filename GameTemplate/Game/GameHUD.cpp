#include "stdafx.h"
#include "GameHUD.h"
#include "player/Player.h"

namespace nsMyGame {

	bool CGameHUD::Start() {

		//HPバーのフレームを作成。
		m_hpBarFrame = NewGO<CSpriteRender>(enPriority_Second);
		m_hpBarFrame->Init("Assets/Image/HP_BarFrame.dds", 400.0f,25.0f);
		m_hpBarFrame->SetPosition({-550.0f,230.0f,0.0f});
		m_hpBarFrame->SetPivot(CVector2::Zero);

		//HPバーの背景を作成。
		m_hpBarBack = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_hpBarBack->Init("Assets/Image/HP_Back.dds", 400.0f, 25.0f);
		m_hpBarBack->SetPosition({ -550.0f,230.0f,0.0f });
		m_hpBarBack->SetPivot(CVector2::Zero);

		//HPバーを作成。
		m_hpBar = NewGO<CSpriteRender>(enPriority_First);
		m_hpBar->Init("Assets/Image/HP.dds", 400.0f, 25.0f);
		m_hpBar->SetPosition({ -550.0f,230.0f,0.0f });
		m_hpBar->SetPivot(CVector2::Zero);

		return true;
	}

	void CGameHUD::OnDestroy() {

		DeleteGO(m_hpBarFrame);
		DeleteGO(m_hpBar);
	}

	void CGameHUD::Update() {

		//HPバーを更新。
		UpdateHPBar();
	}

	void CGameHUD::UpdateHPBar() {

		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);
		float playerHP = player->GetHP();
		m_hpBarScale.x = playerHP / static_cast<float>(nsPlayer::c_playerMaxHP);

		m_hpBar->SetScale(m_hpBarScale);
	}
}