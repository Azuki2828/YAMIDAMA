#include "stdafx.h"
#include "GameTitle.h"
#include "Game.h"

namespace nsMyGame {

	bool CGameTitle::Start() {

		//タイトル画像を初期化。
		m_titleSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_titleSprite->Init(c_filePathTitleSprite, FRAME_BUFFER_W, FRAME_BUFFER_H);

		//「NEW GAME」テキストの初期化。
		m_newGameSprite = NewGO<CSpriteRender>(enPriority_Second);
		m_newGameSprite->Init(c_filePathNewGameSprite, FRAME_BUFFER_W, FRAME_BUFFER_H);
		m_newGameSprite->SetScale({ 0.15f,0.15f,0.15f });
		m_newGameSprite->SetPosition({ 0.0f,-150.0f,0.0f });

		//「NEW GAME」テキストの初期化。
		m_textBackSprite = NewGO<CSpriteRender>(enPriority_First);
		m_textBackSprite->Init(c_filePathTextBackSprite, FRAME_BUFFER_W, FRAME_BUFFER_H);
		m_textBackSprite->SetScale({ 0.4f,0.06f,0.15f });
		m_textBackSprite->SetPosition({ 0.0f,-150.0f,0.0f });

		//ゲーム中のBGMを再生。
		CSoundManager::GetInstance()->Play(enBGM_GameMain);

		//フェードイン
		m_fade = FindGO<CFade>(c_classNameFade);
		m_fade->StartFadeIn();

		return true;
	}

	void CGameTitle::OnDestroy() {

		//タイトル画像を削除。
		DeleteGO(m_titleSprite);
		DeleteGO(m_newGameSprite);
		DeleteGO(m_textBackSprite);
	}

	void CGameTitle::Update() {

		//初めてAボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonA) && !m_finishedFadeOut) {

			//フェードアウトを始める。
			m_fade->StartFadeOut();

			m_finishedFadeOut = true;

			//選択SEを再生。
			CSoundManager::GetInstance()->Play(enSE_Select);
		}

		//フェードアウトが終わったら
		if (m_finishedFadeOut && !m_fade->IsFade()) {

			//ゲームシーンに移行。
			NewGO<CGameMain>(enPriority_Zeroth, c_classNameGameMain);

			//タイトル画面を削除。
			DeleteGO(this);
		}
	}
}