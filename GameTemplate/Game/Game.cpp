#include "stdafx.h"
#include "player/Player.h"
#include "enemy/firstWinEnemy/FirstWinEnemy.h"
#include "enemy/boss/Boss.h"
#include "Game.h"
#include "BackGround.h"
#include "CameraManager.h"
#include "GameTitle.h"
#include "GameHUD.h"

namespace nsMyGame {

	bool CGameMain::Start() {

		//プレイヤーを生成。
		m_player = NewGO<nsPlayer::CPlayer>(enPriority_Zeroth, c_classNamePlayer);

		//背景クラスを生成。
		m_backGround = NewGO<CBackGround>(enPriority_Zeroth, c_classNameBackGround);

		//メインカメラを生成。
		m_cameraManager = NewGO<CCameraManager>(enPriority_Zeroth, c_classNameCameraManager);

		//HUDを作成。
		m_gameHUD = NewGO<CGameHUD>(enPriority_Zeroth, c_classNameGameHUD);

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
		DeleteGO(m_cameraManager);
		DeleteGO(m_gameHUD);
	}

	void CGameMain::Update() {

		//プレイヤーが死んでいるならYouDiedを表示させる。
		if (m_player->IsDeath()) {

			//死亡スプライト用のタイマーを更新。
			m_youDiedMessageTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			//一定時間以上かつ、完全に表示されていないなら
			if (m_youDiedMessageTime >= 2.5f && m_youDiedSpriteTrans < 1.0f) {

				//ゲームオーバーサウンドが再生されていないなら
				if (!m_gameOverSoundFlag) {

					//ゲームオーバーサウンドを再生。
					CSoundManager::GetInstance()->Play(enSE_GameOver);

					//ゲームオーバーサウンドが再生された。
					m_gameOverSoundFlag = true;
				}
				
				//有効にする。
				m_youDiedSprite->Activate();

				//だんだん表示されるようにする。
				m_youDiedSpriteTrans += GameTime().GameTimeFunc().GetFrameDeltaTime() * 0.5f;
				m_youDiedSprite->SetMulColor({ CVector4::White.x,CVector4::White.y,CVector4::White.z, m_youDiedSpriteTrans });
			}
		}
		else if (g_pad[0]->IsTrigger(enButtonRB3)) {

			//ロックオン状態じゃないなら
			if (m_cameraManager->GetCameraType() != enCamera_LockOn) {

				//ロックオン状態に。
				m_cameraManager->SetCameraType(enCamera_LockOn);
			}
			//ロックオン状態なら
			else {
				//通常カメラに。
				m_cameraManager->SetCameraType(enCamera_Main);
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
				m_gameClearSprite->SetMulColor({ CVector4::White.x,CVector4::White.y,CVector4::White.z, m_gameClearSpriteTrans });
			}
		}

		//タイトルに戻る。
		if (m_youDiedMessageTime > 7.0f || m_gameClearMessageTime > 4.5f) {

			if (!m_fade->IsFade()) {
				DeleteGO(this);
				NewGO<CGameTitle>(enPriority_Zeroth);

				CSoundManager::GetInstance()->Release(enBGM_Boss);
			}
		}
		//フェードアウト。
		else if (m_youDiedMessageTime > 6.0f || m_gameClearMessageTime > 3.5f) {

			m_fade->StartFadeOut();
		}
	}
}