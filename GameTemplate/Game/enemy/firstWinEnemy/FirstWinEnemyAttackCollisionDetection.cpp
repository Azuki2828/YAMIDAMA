#include "stdafx.h"
#include "FirstWinEnemyAttackCollisionDetection.h"
#include "../../player/Player.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CFirstWinEnemyAttackCollisionDetection::Update() {

			//ガードフラグを初期化。
			m_isGuarded = false;

			//アクティブじゃないなら終了。
			if (!m_isActive) { return; }

			//プレイヤーを検索。
			auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//プレイヤーの検索に失敗したなら終了。
			if (player == nullptr) { return; }

			//剛体との当たり判定を調べる。
			CPhysicsWorld::GetInstance()->ContactTest(player->GetCharacterController(), [&](const btCollisionObject& contactObject) {

				//トリガーボックスと接触した。
				if (m_attackCollision.IsSelf(contactObject)) {

					//プレイヤーがガード状態ならガードされ、当たり判定をしないように設定する。
					if (player->IsGuard()) {

						m_isGuarded = true;
						m_isActive = false;
					}

					//プレイヤーに攻撃。
					player->JudgeDamage();
				}
			});
		}
	}
}
