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

	//void PlayerSelect::Update() {

	//	//オブジェクトの座標を読み込む。
	//	CVector3 pos = CVector3::Zero;

	//	//プレイヤーを検索。
	//	auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

	//	//プレイヤーの座標を取得。
	//	CVector3 playerPos = player->GetPosition();

	//	//規定値より100.0f大きい数を初期値とする。
	//	float vecToPlayerLength = c_distanceForOpenDoor + 100.0f;

	//	//リストに含まれるオブジェクトを参照。
	//	for (const auto& obj : m_objectList) {

	//		//クラスの名前を調べる。
	//		const std::type_info& typeInfo = typeid(*obj);

	//		//ドアだったら
	//		if (typeInfo == typeid(CDoor)) {

	//			//ドアクラスにキャスト。
	//			auto doorObj = dynamic_cast<CDoor*>(obj);

	//			//プレイヤーに伸びるベクトルを計算。
	//			CVector3 vecToPlayer = playerPos - doorObj->GetPosition();

	//			//プレイヤーとの最短距離を更新。
	//			if (vecToPlayer.Length() < vecToPlayerLength) {

	//				vecToPlayerLength = vecToPlayer.Length();

	//				//最短距離を更新したドアとプレイヤーの距離が一定以下かつ、そのドアが開いていないなら
	//				if (vecToPlayerLength <= c_distanceForOpenDoor && !doorObj->IsOpened()) {

	//					//プレイヤーを選択状態にする。
	//					player->SetSelectFlag(true);
	//					break;
	//				}
	//			}
	//			//それ以外は、プレイヤーは何も選択していない状態。
	//			else {

	//				player->SetSelectFlag(false);
	//			}
	//		}
	//	}
	//}
}