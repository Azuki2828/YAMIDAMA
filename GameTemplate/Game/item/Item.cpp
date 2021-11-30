#include "stdafx.h"
#include "Item.h"
#include "../player/Player.h"

namespace nsMyGame {

	namespace nsItem {

		bool CItem::Start() {

			//エフェクトを初期化。
			m_itemEffect = NewGO<Effect>(enPriority_Zeroth);
			m_itemEffect->Init(c_itemEffectFilePath);
			m_itemEffect->SetPosition(m_position);
			m_itemEffect->SetScale(c_itemEffectSize);

			//再生。
			m_itemEffect->Play();

			return true;
		}

		void CItem::Update() {

			//プレイヤーとの距離が規定以下なら自身が選択されている状態にする。
			if (CheckLengthToPlayer() <= c_needToDistanceForGetItem) {

				m_isSelected = true;
			}
			//そうじゃないなら選択状態から外す。
			else {
				
				m_isSelected = false;
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

			//見つからなかったら規定より1000.0f大きい値を返す。
			if (player == nullptr) { return c_needToDistanceForGetItem + 1000.0f; }

			//プレイヤーの座標を取得。
			CVector3 playerPos = player->GetPosition();

			//プレイヤーに向かうベクトルを求める。
			CVector3 vecToPlayer = playerPos - m_position;

			//プレイヤーとの距離を返す。
			return vecToPlayer.Length();
		}
	}
}