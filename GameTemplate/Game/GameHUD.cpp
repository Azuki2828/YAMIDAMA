#include "stdafx.h"
#include "GameHUD.h"
#include "player/Player.h"

namespace nsMyGame {

	bool CGameHUD::Start() {

		//HPバーを初期化。
		InitHPBar();

		//赤HPバーの減少の仕方を設定。
		m_hpDecreaseType = enType_Linear;

		return true;
	}

	void CGameHUD::OnDestroy() {

		DeleteGO(m_hpBarFrame);
		DeleteGO(m_hpBar_Green);
		DeleteGO(m_hpBar_Red);
		DeleteGO(m_hpBarBack);
		DeleteGO(m_bossHpBarFrame);
		DeleteGO(m_bossHpBarBack);
		DeleteGO(m_bossHpBar);
		DeleteGO(m_bossNameText);
	}

	void CGameHUD::Update() {

		//HPバーを更新。
		UpdateHPBar();
	}

	void CGameHUD::InitHPBar() {

		//HPバーのフレームを作成。
		m_hpBarFrame = NewGO<CSpriteRender>(enPriority_Third);
		m_hpBarFrame->Init("Assets/Image/HP_BarFrame.dds", 400.0f, 25.0f);
		m_hpBarFrame->SetPosition({ -570.0f,300.0f,0.0f });
		m_hpBarFrame->SetPivot(CVector2::Zero);

		//HPバーの背景を作成。
		m_hpBarBack = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_hpBarBack->Init("Assets/Image/HP_Back.dds", 400.0f, 25.0f);
		m_hpBarBack->SetPosition({ -570.0f,300.0f,0.0f });
		m_hpBarBack->SetPivot(CVector2::Zero);

		//緑HPバーを作成。
		m_hpBar_Green = NewGO<CSpriteRender>(enPriority_Second);
		m_hpBar_Green->Init("Assets/Image/HP.dds", 400.0f, 25.0f);
		m_hpBar_Green->SetPosition({ -570.0f,300.0f,0.0f });
		m_hpBar_Green->SetPivot(CVector2::Zero);

		//赤HPバーを作成。
		m_hpBar_Red = NewGO<CSpriteRender>(enPriority_First);
		m_hpBar_Red->Init("Assets/Image/HP_2.dds", 400.0f, 25.0f);
		m_hpBar_Red->SetPosition({ -570.0f,300.0f,0.0f });
		m_hpBar_Red->SetPivot(CVector2::Zero);

		//ボスのHPバーのフレームを作成。
		m_bossHpBarFrame = NewGO<CSpriteRender>(enPriority_Second);
		m_bossHpBarFrame->Init("Assets/Image/HP_BarFrame.dds", 400.0f, 25.0f);
		m_bossHpBarFrame->SetPosition({ -400.0f,-300.0f,0.0f });
		m_bossHpBarFrame->SetPivot(CVector2::Zero);
		m_bossHpBarFrame->Deactivate();
		
		//ボスのHPバーの背景を作成。
		m_bossHpBarBack = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_bossHpBarBack->Init("Assets/Image/HP_Back.dds", 400.0f, 25.0f);
		m_bossHpBarBack->SetPosition({ -400.0f,-300.0f,0.0f });
		m_bossHpBarBack->SetPivot(CVector2::Zero);
		m_bossHpBarBack->Deactivate();

		//ボスのHPバーを作成。
		m_bossHpBar = NewGO<CSpriteRender>(enPriority_First);
		m_bossHpBar->Init("Assets/Image/HP_2.dds", 400.0f, 25.0f);
		m_bossHpBar->SetPosition({ -400.0f,-300.0f,0.0f });
		m_bossHpBar->SetPivot(CVector2::Zero);
		m_bossHpBar->Deactivate();

		m_bossNameText = NewGO<nsFont::CFontRender>(enPriority_Zeroth);
		m_bossNameText->Init(L"原初の門番");
		m_bossNameText->SetColor({ 0.8f,0.8f,0.8f,1.0f });
		m_bossNameText->SetScale(0.5f);
		m_bossNameText->SetPosition({ -400.0f,-250.0f });
		m_bossNameText->Deactivate();
	}

	void CGameHUD::UpdateHPBar() {

		//プレイヤーを検索。
		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

		//プレイヤーのHPを取得。
		float playerHP = player->GetHP();

		//HPゲージを更新。
		m_hpBarScale.x = playerHP / static_cast<float>(nsPlayer::c_playerMaxHP);

		//拡大率を設定。
		m_hpBar_Green->SetScale(m_hpBarScale);

		//赤HPバーを更新。
		UpdateRedHPBar();
	}

	void CGameHUD::UpdateRedHPBar() {
		
		if (m_hpBar_Green->GetScale().x < m_hpBar_Red->GetScale().x) {

			//タイマーを更新。
			m_damageTimer += GameTime().GetFrameDeltaTime();

			//一定時間が経ったら赤HPゲージも減るように。
			if (m_damageTimer > 2.0f) {

				//更新できるなら
				if (m_canHPBarScaleUpdate) {

					//赤HPゲージの減少の拡大率を更新。
					m_damageScale = m_hpBar_Red->GetScale().x - m_hpBar_Green->GetScale().x;
				}

				//非線形に設定しているなら
				if (m_hpDecreaseType == enType_NonLinear) {

					//これ以上赤HPゲージの減少の拡大率の更新をしないようにする。
					m_canHPBarScaleUpdate = false;
				}

				//赤HPバーの拡大率を更新。
				m_hpBar_Red->SetScale({ m_hpBar_Red->GetScale().x - m_damageScale / 20.0f,1.0f,1.0f });
			}

			//赤HPバーと緑HPバーの拡大率が限りなく近いなら、同じに設定する。
			if (m_hpBar_Red->GetScale().x - m_hpBar_Green->GetScale().x < 0.001f) {
				m_hpBar_Red->SetScale({ m_hpBar_Green->GetScale().x,1.0f,1.0f });
			}
		}
		//処理しないならタイマーと更新フラグを初期化する。
		else {
			m_damageTimer = 0.0f;
			m_canHPBarScaleUpdate = true;
		}
	}
}