#include "stdafx.h"
#include "AppearSprite.h"
#include "player/Player.h"

namespace nsMyGame {

	bool CAppearSprite::Start() {

		//テキストのスプライトを初期化。
		m_textSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_textSprite->Init(c_filePathTextSprite, c_textSpriteWH.x, c_textSpriteWH.y);
		m_textSprite->SetPosition(c_textSpritePosition);
		m_textSprite->SetScale(c_textSpriteSize);

		//非表示に設定。
		m_textSprite->Deactivate();

		//テキストを初期化。
		m_font = NewGO<nsFont::CFontRender>(enPriority_Zeroth);
		m_font->Init(m_text);
		m_font->SetPosition(m_textPosition);
		m_font->SetScale(c_textSize);
		m_font->SetColor(CVector4::White);

		//非表示に設定。
		m_font->Deactivate();

		return true;
	}

	void CAppearSprite::OnDestroy() {

		DeleteGO(m_textSprite);
		DeleteGO(m_font);
	}

	void CAppearSprite::Update() {

		//プレイヤーを検索。
		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

		//プレイヤーを選択状態に設定。
		player->SetSelectFlag(true);

		//Aボタンが入力されたら確認フラグをtrueにする。
		if (g_pad[0]->IsTrigger(enButtonA)) {

			m_isPressButton = true;
		}

		//確認ボタンが入力されていないなら
		if (!IsPressButton()) {

			//スプライトとテキストを徐々に出現させる。
			AppearSpriteAndText();
		}
		//確認ボタンが入力されたら
		else {

			//だんだんスプライトが消えるようにする。
			DisappearSpriteAndText();

			//完全に消えたら自身を削除。
			if (m_textSpriteTranslucent == c_translucentValue_Zero.w) {
				DeleteGO(this);
				DeleteGO(m_textSprite);
				DeleteGO(m_font);
			}
		}
	}

	void CAppearSprite::AppearSpriteAndText() {

		//スプライトとテキストを表示。
		m_textSprite->Activate();
		m_font->Activate();

		//だんだんスプライトが現れるようにする。
		if (m_textSpriteTranslucent < c_translucentValue_Max.w) {

			//テキストカラーを設定。
			float textColor = m_textSpriteTranslucent;
			m_font->SetColor({ textColor ,textColor ,textColor,m_textSpriteTranslucent });

			//スプライトの透明度を設定。
			m_textSpriteTranslucent += GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
			m_textSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_textSpriteTranslucent });
		}
	}

	void CAppearSprite::DisappearSpriteAndText() {

		if (m_textSpriteTranslucent > c_translucentValue_Zero.w) {

			//テキストカラーを設定。
			float textColor = m_textSpriteTranslucent;
			m_font->SetColor({ textColor,textColor,textColor,m_textSpriteTranslucent });

			//スプライトの透明度を設定。
			m_textSpriteTranslucent -= GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
			m_textSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_textSpriteTranslucent });
		}
		//開くスプライトを非表示。
		//テキストを非表示。
		else {
			m_textSpriteTranslucent = c_translucentValue_Zero.w;
			m_textSprite->Deactivate();
			m_font->Deactivate();
		}
	}
}