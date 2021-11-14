#include "stdafx.h"
#include "GoteWinEnemyAttackCollisionDetection.h"
#include "../../player/Player.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CGoteWinEnemyCollisionDetection::Create(const CVector3& pos, const CQuaternion& rot) {

			//攻撃の当たり判定を作成。
			m_triggerBox.CreateBox(
				pos,
				rot,
				c_attackTriggerBoxSize
			);
		}

		void CGoteWinEnemyCollisionDetection::Activate(const CVector3& pos, const CQuaternion& rot) {

			if (!m_isActive) {

				//トリガーボックスを作成。
				Create(pos, rot);

				m_player = FindGO<nsPlayer::CPlayer>("player");

				m_player->SetReceiveDamage(false);

				m_isActive = true;
			}
		}

		void CGoteWinEnemyCollisionDetection::Deactivate() {

			if (m_isActive) {

				//トリガーボックスを削除。
				m_triggerBox.Release();
				m_isActive = false;
			}
		}


		void CGoteWinEnemyCollisionDetection::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& forward) {

			//アクティブじゃないなら終了。
			if (!m_isActive) {

				return;
			}

			//座標を求める。
			CVector3 position = pos;
			position.y += c_attackTriggerBoxSize.y / 2;

			//座標と回転を設定。
			m_triggerBox.SetPosition(position + forward * c_attackTriggerBoxMul);
			m_triggerBox.SetRotation(rot);

			//剛体との当たり判定を調べる。
			CPhysicsWorld::GetInstance()->ContactTest(m_player->GetCharacterController(), [&](const btCollisionObject& contactObject) {

				//まだプレイヤーが今回の攻撃を受けていない状態でトリガーボックスと接触した。
				if (!m_player->GetReceiveDamage() && m_triggerBox.IsSelf(contactObject)) {

					if (m_player->IsGuard()) {

						m_isGuarded = true;
					}
					else {
					
						m_isGuarded = false;
					}

					//プレイヤーにダメージを与える。
					m_player->SetReceiveDamage(true);
				}
			});
		}
	}
}
