#include "stdafx.h"
#include "PlayerSelect.h"
#include "../Door.h"
#include "Player.h"

namespace nsMyGame {

	void CPlayerSelect::SetSelectObject(CIGameObject* selectObj, const CVector3& playerPos) {

		//格納されていないならそのまま選択状態にする。
		if (m_selectObject == nullptr) {

			m_selectObject = selectObj;
			selectObj->SetSelectFlag(true);
		}
		//既に何かを選択している
		else {

			//プレイヤーに伸びるベクトルを計算。
			CVector3 vecToPlayer = playerPos - selectObj->GetPosition();

			float length = vecToPlayer.Length();
			float length2 = GetSelectObjectLengthToPlayer(playerPos);

			//現在選択しているオブジェクトとプレイヤーとの距離と、
			//今回のオブジェクトとプレイヤーとの距離を比較して、より短かった方を選択状態にする。
			if (vecToPlayer.Length() < GetSelectObjectLengthToPlayer(playerPos)) {

				m_selectObject->SetSelectFlag(false);
				m_selectObject = selectObj;
				selectObj->SetSelectFlag(true);
			}
		}
	}

	const float CPlayerSelect::GetSelectObjectLengthToPlayer(const CVector3& playerPos) {

		//登録されていないなら負の値を返す。
		if (m_selectObject == nullptr) {

			return -1.0f;
		}

		//プレイヤーに伸びるベクトルを計算。
		CVector3 vecToPlayer = playerPos - m_selectObject->GetPosition();

		//プレイヤーとの距離を返す。
		return vecToPlayer.Length();
	}
}