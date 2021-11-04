#include "stdafx.h"
#include "EnemyAction.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CEnemyAction::Init(const CVector3& position, CharacterController& charaCon) {

			m_charaCon = &charaCon;

			//キャラクターコントローラーを初期化。
			m_charaCon->Init(
				20.0f,			//半径。
				200.0f,			//高さ。
				position		//座標。
			);
		}

		/*void CEnemyAction::Move(CVector3& position, CVector3& forward, EnEnemyState& enemyState) {


		}*/

		void CEnemyAction::Rotate(const CQuaternion& rotation) {

		}

		

		void CEnemyAction::Update() {

		}
	}
}