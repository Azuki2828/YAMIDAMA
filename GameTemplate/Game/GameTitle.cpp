#include "stdafx.h"
#include "GameTitle.h"
#include "Game.h"

namespace nsMyGame {

	bool CGameTitle::Start() {

		//タイトル画像を初期化。
		m_titleSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_titleSprite->Init(c_filePathTitleSprite, FRAME_BUFFER_W, FRAME_BUFFER_H);

		return true;
	}

	void CGameTitle::OnDestroy() {

		//タイトル画像を削除。
		DeleteGO(m_titleSprite);
	}

	void CGameTitle::Update() {

		//Aボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonA)) {

			//ゲームシーンに移行。
			NewGO<CGameMain>(enPriority_Zeroth);

			//タイトル画面を削除。
			DeleteGO(this);
		}
	}
}