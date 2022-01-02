#include "stdafx.h"
#include "PlayerTriggerBox.h"
#include "../enemy/Enemy.h"
#include "../enemy/boss/Boss.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerTriggerBox::Update() {

			//アクティブ状態じゃないなら終了。
			if (!m_isActive) { return; }

			//名前がEnemyのオブジェクトをCEnemyクラスにキャスト。
			QueryGOs<nsEnemy::CEnemy>(c_classNameEnemy, [this](nsEnemy::CEnemy* enemy) {

				//剛体との当たり判定を調べる。
				CPhysicsWorld::GetInstance()->ContactTest(enemy->GetCharacterController(), [&](const btCollisionObject& contactObject) {

					//まだ敵が今回の攻撃を受けていない状態でトリガーボックスと接触した。
					if (m_attackCollision.IsSelf(contactObject)) {

						//敵にダメージを与える。
						enemy->SetReceiveDamage(true);

						//血しぶきエフェクトを再生。
						Effect* bloodEffect = NewGO<Effect>(enPriority_Zeroth);
						bloodEffect->Init(u"Assets/effect/bloodGreen.efk");
						bloodEffect->SetPosition(m_position);
						bloodEffect->SetScale({ 5.0f, 5.0f,5.0f });
						bloodEffect->Play();

						//斬るサウンドを再生。
						CSoundManager::GetInstance()->Play(enSE_Kill);

						m_isActive = false;
					}
				});

				return true;
			});

			auto boss = FindGO<nsEnemy::CBoss>(c_classNameBoss);

			//剛体との当たり判定を調べる。
			CPhysicsWorld::GetInstance()->ContactTest(boss->GetCharacterController(), [&](const btCollisionObject& contactObject) {

				//まだ敵が今回の攻撃を受けていない状態でトリガーボックスと接触した。
				if (m_attackCollision.IsSelf(contactObject)) {

					//敵にダメージを与える。
					boss->SetReceiveDamage(true);

					//血しぶきエフェクトを再生。
					Effect* bloodEffect = NewGO<Effect>(enPriority_Zeroth);
					bloodEffect->Init(u"Assets/effect/bloodGreen.efk");
					bloodEffect->SetPosition(m_position);
					bloodEffect->SetScale({ 5.0f, 5.0f,5.0f });
					bloodEffect->Play();

					//斬るサウンドを再生。
					CSoundManager::GetInstance()->Play(enSE_Kill);

					m_isActive = false;
				}
			});
		}
	}
}