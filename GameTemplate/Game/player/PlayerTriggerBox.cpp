#include "stdafx.h"
#include "PlayerTriggerBox.h"
#include "../enemy/Enemy.h"

namespace nsMyGame {

	namespace nsPlayer {
		
		void CPlayerTriggerBox::Create(const CVector3& pos, const CQuaternion& rot) {

			//攻撃の当たり判定を作成。
			m_ghostBox.CreateBox(
				pos,
				rot,
				c_attackTriggerBoxSize
			);
		}

		void CPlayerTriggerBox::Activate(const CVector3& pos, const CQuaternion& rot) {

			if (!m_isActive) {

				//トリガーボックスを作成。
				Create(pos, rot);

				//名前がEnemyのオブジェクトをCEnemyクラスにキャスト。
				//敵がまだ今回の攻撃を受けていないように設定。
				QueryGOs<nsEnemy::CEnemy>(c_classNameEnemy, [this](nsEnemy::CEnemy* enemy) {

					enemy->SetReceiveDamage(false);
					return true;
				});

				m_isActive = true;
			}
		}

		void CPlayerTriggerBox::Deactivate() {
			
			if (m_isActive) {

				m_ghostBox.Release();
				m_isActive = false;
			}
		}


		void CPlayerTriggerBox::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& forward) {

			if (m_isActive) {

				//座標を求める。
				CVector3 position = pos;
				position.y += c_attackTriggerBoxSize.y / 2;

				//座標と回転を設定。
				m_ghostBox.SetPosition(position + forward * c_attackTriggerBoxMul);
				m_ghostBox.SetRotation(rot);

				//名前がEnemyのオブジェクトをCEnemyクラスにキャスト。
				QueryGOs<nsEnemy::CEnemy>(c_classNameEnemy, [this](nsEnemy::CEnemy* enemy) {

					//剛体との当たり判定を調べる。
					CPhysicsWorld::GetInstance()->ContactTest(enemy->GetCharacterController(), [&](const btCollisionObject& contactObject) {

						//まだ敵が今回の攻撃を受けていない状態でトリガーボックスと接触した。
						if (!enemy->GetReceiveDamage() && m_ghostBox.IsSelf(contactObject)) {

							//敵を削除。
							//enemy->Delete();
							//敵にダメージを与える。
							enemy->SetReceiveDamage(true);
						}
					});

					return true;
				});
			}
		}
	}
}