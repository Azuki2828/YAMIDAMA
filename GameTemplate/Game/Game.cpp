#include "stdafx.h"
#include "player/Player.h"
#include "enemy/firstWinEnemy/FirstWinEnemy.h"
#include "enemy/boss/Boss.h"
#include "Game.h"
#include "BackGround.h"
#include "MainCamera.h"

namespace nsMyGame {

	bool CGameMain::Start() {

		//プレイヤーを生成。
		m_player = NewGO<nsPlayer::CPlayer>(enPriority_Zeroth, c_classNamePlayer);

		//背景クラスを生成。
		m_backGround = NewGO<CBackGround>(enPriority_Zeroth, c_classNameBackGround);

		//メインカメラを生成。
		m_mainCamera = NewGO<CMainCamera>(enPriority_Zeroth);

		//死亡スプライトを生成。
		m_youDiedSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_youDiedSprite->Init(c_filePathYouDiedSprite, c_spriteYouDiedWH.x, c_spriteYouDiedWH.y);
		m_youDiedSprite->SetScale(c_spriteSizeYouDied);
		m_youDiedSprite->SetMulColor(c_translucentValue_Zero);

		//非表示に設定。
		m_youDiedSprite->Deactivate();

		// とりあえずテストで敵を追加。
		auto fEnemy = NewGO<nsEnemy::CBoss>(enPriority_Zeroth, c_classNameEnemy);
		fEnemy->SetPosition({ 500.0f,500.0f,500.0f });

		//フェードイン。
		m_fade = FindGO<CFade>(c_classNameFade);
		m_fade->StartFadeIn();

		return true;
	}

	void CGameMain::OnDestroy() {

		//各クラスを削除。
		DeleteGO(m_player);
		DeleteGO(m_youDiedSprite);
		DeleteGO(m_backGround);
		DeleteGO(m_mainCamera);
	}

	void CGameMain::Update() {

		//死亡スプライト用のタイマーを初期化。
		static float youDiedMessageTime = 0.0f;

		//プレイヤーが死んでいるならYouDiedを表示させる。
		if (m_player->IsDeath()) {

			//有効にする。
			m_youDiedSprite->Activate();

			//死亡スプライト用のタイマーを更新。
			youDiedMessageTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			//一定時間以上かつ、完全に表示されていないなら
			if (youDiedMessageTime >= 2.5f && m_youDiedSpriteTrans < 1.0f) {

				//だんだん表示されるようにする。
				m_youDiedSpriteTrans += GameTime().GameTimeFunc().GetFrameDeltaTime() * 0.5f;
				m_youDiedSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, m_youDiedSpriteTrans });
			}
		}
		//死んでいないなら
		else {

			//死亡スプライト用のタイマーを初期化。
			youDiedMessageTime = 0.0f;
		}
	}
}