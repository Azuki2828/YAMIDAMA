#include "stdafx.h"
#include "BossAttackCollisionDetection.h"
#include "../../player/Player.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CBossCollisionDetection::Update(const CVector3& pos) {

			//範囲攻撃用の当たり判定の座標を更新。
			m_jumpAttackCollision.SetPosition(pos);

			//ガードフラグを初期化。
			m_isGuarded = false;

			//プレイヤーを検索。
			auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//プレイヤーの検索に失敗したなら終了。
			if (player == nullptr) { return; }

			//剛体との当たり判定を調べる。
			CPhysicsWorld::GetInstance()->ContactTest(player->GetCharacterController(), [&](const btCollisionObject& contactObject) {

				//トリガーボックスと接触した。
				if (m_isAttackActive && m_attackCollision.IsSelf(contactObject)) {

					//プレイヤーがガード状態ならガードされ、当たり判定をしないように設定する。
					if (player->IsGuard()) {

						m_isGuarded = true;
					}
					m_isAttackActive = false;
					//プレイヤーに攻撃。
					player->JudgeDamage(pos);
				}

				if (m_isRangeAttackActive && m_jumpAttackCollision.IsSelf(contactObject)) {

					//プレイヤーがガード状態ならガードされ、当たり判定をしないように設定する。
					if (player->IsGuard()) {

						m_isGuarded = true;					
					}

					//プレイヤーに攻撃。
					player->JudgeDamage(pos);
					m_isRangeAttackActive = false;
				}
			});
		}
	}
}
