#include "stdafx.h"
#include "player/Player.h"
#include "enemy/firstWinEnemy/FirstWinEnemy.h"
#include "enemy/boss/Boss.h"
#include "Game.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "GameTitle.h"

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

		//ゲームクリアスプライトを生成。
		m_gameClearSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_gameClearSprite->Init(c_filePathGameClearSprite, c_spriteYouDiedWH.x, c_spriteYouDiedWH.y);
		m_gameClearSprite->SetScale(c_spriteSizeYouDied);
		m_gameClearSprite->SetMulColor(c_translucentValue_Zero);

		//非表示に設定。
		m_gameClearSprite->Deactivate();

		//CSoundManager::GetInstance()->Play(enBGM_Boss);
		// とりあえずテストで敵を追加。
		//auto fEnemy = NewGO<nsEnemy::CBoss>(enPriority_Zeroth, c_classNameEnemy);
		//fEnemy->SetPosition({ 500.0f,500.0f,500.0f });

		//フェードイン。
		m_fade = FindGO<CFade>(c_classNameFade);
		m_fade->StartFadeIn();

		return true;
	}

	void CGameMain::OnDestroy() {

		//各クラスを削除。
		DeleteGO(m_player);
		DeleteGO(m_youDiedSprite);
		DeleteGO(m_gameClearSprite);
		DeleteGO(m_backGround);
		DeleteGO(m_mainCamera);
	}

	void CGameMain::Update() {

		//プレイヤーが死んでいるならYouDiedを表示させる。
		if (m_player->IsDeath()) {

			//有効にする。
			m_youDiedSprite->Activate();

			//死亡スプライト用のタイマーを更新。
			m_youDiedMessageTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			//一定時間以上かつ、完全に表示されていないなら
			if (m_youDiedMessageTime >= 2.5f && m_youDiedSpriteTrans < 1.0f) {

				//だんだん表示されるようにする。
				m_youDiedSpriteTrans += GameTime().GameTimeFunc().GetFrameDeltaTime() * 0.5f;
				m_youDiedSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, m_youDiedSpriteTrans });
			}
		}

		//ボスを倒したらゲームクリア画像を表示させる。
		if (m_isGameClear) {

			//有効にする。
			m_gameClearSprite->Activate();

			//死亡スプライト用のタイマーを更新。
			m_gameClearMessageTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			//完全に表示されていないなら
			if (m_gameClearSpriteTrans < 1.0f) {

				//だんだん表示されるようにする。
				m_gameClearSpriteTrans += GameTime().GameTimeFunc().GetFrameDeltaTime() * 0.5f;
				m_gameClearSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, m_gameClearSpriteTrans });
			}
		}

		//タイトルに戻る。
		if (m_youDiedMessageTime > 7.0f || m_gameClearMessageTime > 4.5f) {

			if (!m_fade->IsFade()) {
				DeleteGO(this);
				NewGO<CGameTitle>(enPriority_Zeroth);
			}
		}
		//フェードアウト。
		else if (m_youDiedMessageTime > 6.0f || m_gameClearMessageTime > 3.5f) {

			m_fade->StartFadeOut();
		}
	}
}