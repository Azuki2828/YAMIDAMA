#include "stdafx.h"
#include "player/Player.h"
#include "enemy/firstWinEnemy/FirstWinEnemy.h"
#include "Game.h"
#include "BackGround.h"
#include "MainCamera.h"

namespace nsMyGame {

	bool CGameMain::Start() {

		//プレイヤーを生成。
		m_player = NewGO<nsPlayer::CPlayer>(0, c_classNamePlayer);

		//背景クラスを生成。
		m_backGround = NewGO<CBackGround>(0, c_classNameBackGround);

		//メインカメラを生成。
		m_mainCamera = NewGO<CMainCamera>(0);

		m_youDiedSprite = NewGO<CSpriteRender>(0);
		m_youDiedSprite->Init("Assets/image/youDied2.dds", 1740.0f, 180.0f);
		m_youDiedSprite->SetPosition({ 0.0f,0.0f,0.0f });
		m_youDiedSprite->SetScale({ 0.8f,0.8f,0.8f });
		m_youDiedSprite->SetMulColor({ 1.0f,1.0f,1.0f,m_youDiedSpriteTrans });

		//非表示に設定。
		m_youDiedSprite->Deactivate();

		// とりあえずテストで敵を追加。
		auto fEnemy = NewGO<nsEnemy::CFirstWinEnemy>(0, c_classNameEnemy);
		fEnemy->SetPosition({ 500.0f,500.0f,500.0f });

		return true;
	}

	void CGameMain::OnDestroy() {

		DeleteGO(m_player);
		DeleteGO(m_youDiedSprite);
		DeleteGO(m_backGround);
		DeleteGO(m_mainCamera);
	}

	void CGameMain::Update() {

		//プレイヤーが死んでいるならYouDiedを表示させる。
		if (m_player->IsDeath()) {

			//有効にする。
			m_youDiedSprite->Activate();

			static float youDiedMessageTime = 0.0f;

			youDiedMessageTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			if (youDiedMessageTime >= 3.0f && m_youDiedSpriteTrans < 1.0f) {

				m_youDiedSpriteTrans += 0.01f;
				m_youDiedSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, m_youDiedSpriteTrans });
			}
		}
	}
}