#include "stdafx.h"
#include "Item.h"
#include "../player/Player.h"
#include "../AppearSprite.h"

namespace nsMyGame {

	namespace nsItem {

		bool CItem::Start() {

			//エフェクトを初期化。
			InitEffect();

			//メッセージウィンドウを初期化。
			InitSpriteAndText();

			return true;
		}

		void CItem::Update() {

			//プレイヤーを検索。
			auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//プレイヤーとの距離が規定以下なら自身が選択されている状態にする。
			if (CheckLengthToPlayer() <= c_needToDistanceForGetItem) {

				//プレイヤーを選択状態に設定。
				player->SetSelectFlag(true);
				m_isSelected = true;
			}
			//そうじゃないなら選択状態から外す。
			else {
				
				m_isSelected = false;
			}

			//自身が選択状態にあり、かつAボタンが入力されたら
			if (IsSelected()){

				//スプライトとテキストを徐々に出現させる。
				AppearSpriteAndText();
				
				if (g_pad[0]->IsTrigger(enButtonA)) {

					//アイテムの種類
					switch (m_item) {
					case enItem_Key:	//鍵

						//プレイヤーの鍵の所持数を+1する。
						player->GetKey();

						//鍵取得ウィンドウを生成。
						auto getSprite = NewGO<CAppearSprite>(enPriority_Zeroth, c_classNameAppearSprite);
						getSprite->SetText(L"[Key] get. A: OK");
						getSprite->SetTextPosition(c_textPosition_getKey);
						break;
					}

					//自信を削除。
					DeleteGO(this);
					DeleteGO(m_textSprite);
					DeleteGO(m_text);

					//エフェクトを削除。
					DeleteGO(m_itemEffect);
				}
			}
			else {

				//だんだんスプライトが消えるようにする。
				DisappearSpriteAndText();
			}

			//エフェクトを更新。
			EffectUpdate();
		}

		void CItem::EffectUpdate() {

			//エフェクトを再生成するための時間を更新。
			m_itemEffectReplayTime += GameTime().GameTimeFunc().GetFrameDeltaTime();

			//規定時間ごとにエフェクトを追加再生。
			if (m_itemEffectReplayTime >= c_itemEffectReplayTime) {

				m_itemEffect->Play();
				m_itemEffectReplayTime = 0.0f;
			}
		}

		const float CItem::CheckLengthToPlayer()const {

			//プレイヤーを検索。
			auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//見つからなかったら規定値より1000.0f大きい値を返す。
			if (player == nullptr) { return c_needToDistanceForGetItem + 1000.0f; }

			//プレイヤーの座標を取得。
			CVector3 playerPos = player->GetPosition();

			//プレイヤーに向かうベクトルを求める。
			CVector3 vecToPlayer = playerPos - m_position;

			//プレイヤーとの距離を返す。
			return vecToPlayer.Length();
		}

		void CItem::AppearSpriteAndText() {

			//スプライトとテキストを表示。
			m_textSprite->Activate();
			m_text->Activate();

			//だんだんスプライトが現れるようにする。
			if (m_textSpriteTranslucent < c_translucentValue_Max.w) {

				//テキストカラーを設定。
				float textColor = m_textSpriteTranslucent;
				m_text->SetColor({ textColor ,textColor ,textColor,m_textSpriteTranslucent });

				//スプライトの透明度を設定。
				m_textSpriteTranslucent += GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
				m_textSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_textSpriteTranslucent });
			}
		}

		void CItem::DisappearSpriteAndText() {

			if (m_textSpriteTranslucent > c_translucentValue_Zero.w) {

				//テキストカラーを設定。
				float textColor = m_textSpriteTranslucent;
				m_text->SetColor({ textColor,textColor,textColor,m_textSpriteTranslucent });

				//スプライトの透明度を設定。
				m_textSpriteTranslucent -= GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
				m_textSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_textSpriteTranslucent });
			}
			//開くスプライトを非表示。
			//テキストを非表示。
			else {
				m_textSpriteTranslucent = c_translucentValue_Zero.w;
				m_textSprite->Deactivate();
				m_text->Deactivate();
			}
		}

		void CItem::InitEffect() {

			//エフェクトを初期化。
			m_itemEffect = NewGO<Effect>(enPriority_Zeroth);
			m_itemEffect->Init(c_itemEffectFilePath);
			m_itemEffect->SetPosition(m_position);
			m_itemEffect->SetScale(c_itemEffectSize);

			//再生。
			m_itemEffect->Play();
		}

		void CItem::InitSpriteAndText() {

			//テキストのスプライトを初期化。
			m_textSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
			m_textSprite->Init(c_filePathTextSprite, c_textSpriteWH.x, c_textSpriteWH.y);
			m_textSprite->SetPosition(c_textSpritePosition);
			m_textSprite->SetScale(c_textSpriteSize);

			//非表示に設定。
			m_textSprite->Deactivate();

			//テキストを設定。
			m_text = NewGO<nsFont::CFontRender>(enPriority_Zeroth);
			m_text->Init(L"A: Pick up");
			m_text->SetPosition(c_textPosition);
			m_text->SetScale(c_textSize);
			m_text->SetColor(CVector4::White);

			//非表示に設定。
			m_text->Deactivate();
		}
	}
}