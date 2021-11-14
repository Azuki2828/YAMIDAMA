#include "stdafx.h"
#include "FirstWinEnemyAttackCollisionDetection.h"
#include "../../player/Player.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CFirstWinEnemyAttackCollisionDetection::Create(const CVector3& pos, const CQuaternion& rot) {

			//攻撃の当たり判定を作成。
			m_ghostBox.CreateBox(
				pos,
				rot,
				c_attackTriggerBoxSize
			);
			
		}

		void CFirstWinEnemyAttackCollisionDetection::Activate(const CVector3& pos, const CQuaternion& rot) {

			if (!m_isActive) {

				//トリガーボックスを作成。
				Create(pos, rot);

				//プレイヤーを探す。
				m_player = FindGO<nsPlayer::CPlayer>("player");

				//ダメージフラグをfalseに設定。
				m_player->SetReceiveDamage(false);

				//トリガーボックスを有効にする。
				m_isActive = true;
			}
		}

		void CFirstWinEnemyAttackCollisionDetection::Deactivate() {

			if (m_isActive) {

				//トリガーボックスを削除。
				m_ghostBox.Release();
				//トリガーボックスを無効にする。
				m_isActive = false;
				m_isGuarded = false;
			}
		}


		void CFirstWinEnemyAttackCollisionDetection::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& forward) {

			//アクティブじゃないなら終了。
			if (!m_isActive) {

				return;
			}

			//座標を求める。
			CVector3 position = pos;
			position.y += c_attackTriggerBoxSize.y / 2;

			//座標と回転を設定。
			m_ghostBox.SetPosition(position + forward * c_attackTriggerBoxMul);
			m_ghostBox.SetRotation(rot);

			//剛体との当たり判定を調べる。
			CPhysicsWorld::GetInstance()->ContactTest(m_player->GetCharacterController(), [&](const btCollisionObject& contactObject) {

				//まだプレイヤーが今回の攻撃を受けていない状態でトリガーボックスと接触した。
				if (!m_player->GetReceiveDamage() && m_ghostBox.IsSelf(contactObject)) {

					if (m_player->IsGuard()) {

						m_isGuarded = true;
						return;
					}
					else {

						//プレイヤーにダメージを与える。
						m_player->SetReceiveDamage(true);
					}
				}
			});
		}
	}
}
