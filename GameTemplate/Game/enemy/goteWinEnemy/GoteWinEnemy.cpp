#include "stdafx.h"
#include "GoteWinEnemy.h"

namespace nsMyGame {

	namespace nsEnemy {

		extern CEnemy* g_pCurrentEnemy;

		bool CGoteWinEnemy::StartSub() {

			//IGameObjectに追加。
			m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

			//tkmファイルとtksファイルを設定。
			m_modelRender->SetFilePathTkm(c_filePathTkmEnemy);
			m_modelRender->SetFilePathTks(c_filePathTksEnemy);

			//アニメーションクリップを初期化。
			InitAnimationClip();

			//アニメーションを初期化。
			m_modelRender->InitAnimation(m_animationClip, enAnim_Num);

			//影を落とす。
			m_modelRender->SetShadowCasterFlag(true);

			//影を受ける。
			m_modelRender->SetShadowReceiverFlag(true);

			//ステータスを初期化。
			InitStatus();

			//初期化。
			m_modelRender->Init();
			return true;
		}

		void CGoteWinEnemy::UpdateSub() {

			// 現在更新処理を実行中のエネミーのアドレスを代入
			g_pCurrentEnemy = this;

			//ステートに応じて読み込むPythonスクリプトを変える。
			switch (m_state) {
			case enState_Idle:
				ImportModule("GoteWinEnemyIdle");
				break;
			case enState_Walk:
				ImportModule("GoteWinEnemyMove");
				break;
			case enState_Attack:
				ImportModule("GoteWinEnemyAttack");
				break;
			case enState_Damage:
				ImportModule("GoteWinEnemyDamage");
				break;
			case enState_Death:
				ImportModule("GoteWinEnemyDeath");
				break;
			}

			//スクリプトのUpdate()関数を呼び出す。
			auto updateFunc = m_enemyPyModule.attr("Update");
			updateFunc();
		}

		void CGoteWinEnemy::InitStatus() {

			m_status.hp = 100;
			m_status.attack = 10;
		}

		void CGoteWinEnemy::InitAnimationClip() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/goteWinEnemyIdle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Attack].Load("Assets/animData/attack.tka");
			m_animationClip[enAnim_Attack].SetLoopFlag(false);
			m_animationClip[enAnim_Guard].Load("Assets/animData/guard.tka");
			m_animationClip[enAnim_Guard].SetLoopFlag(false);
			m_animationClip[enAnim_Damage].Load("Assets/animData/damage.tka");
			m_animationClip[enAnim_Damage].SetLoopFlag(false);
			m_animationClip[enAnim_Death].Load("Assets/animData/death.tka");
			m_animationClip[enAnim_Death].SetLoopFlag(false);
		}

		void CGoteWinEnemy::AnimationUpdate() {

			switch (m_state) {
			case enState_Idle:
				m_modelRender->PlayAnimation(enAnim_Idle, 0.4f);
				break;
			case enState_Walk:
				m_modelRender->PlayAnimation(enAnim_Walk, 0.4f);
				break;
			case enState_Attack:
				m_modelRender->PlayAnimation(enAnim_Attack, 0.4f);
				break;
			case enState_Guard:
				m_modelRender->PlayAnimation(enAnim_Guard, 0.4f);
				break;
			case enState_Damage:
				m_modelRender->PlayAnimation(enAnim_Damage, 0.4f);
				break;
			case enState_Death:
				m_modelRender->PlayAnimation(enAnim_Death, 0.4f);
				break;
			}
		}

		void CGoteWinEnemy::Move() {

			//x方向とz方向の移動速度を初期化。
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;

			//プレイヤーの座標を取得する。
			CVector3 toPlayerVec = m_player->GetPosition() - m_position;
			//正規化。
			toPlayerVec.Normalize();

			if (m_state == enState_Walk) {

				m_moveSpeed += toPlayerVec * 150.0f;
			}
			else if (m_state == enState_Attack) {

				if (c_threeComboCoolTime - m_coolTime < 2.4f) {
					m_moveSpeed += toPlayerVec * 50.0f;
				}
			}
			//重力をかける。
			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

			//座標を設定。
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}
		}

		void CGoteWinEnemy::UpdateTriggerBox(const CVector3& pos, const CQuaternion& rot, const CVector3& forward) {

			if (m_state == enState_Attack) {

				//斬るタイミングでトリガーボックスを有効にする。
				if (m_coolTime > 0.2f && m_coolTime < 0.6f) {

					m_triggerBox.Activate(pos, rot);
				}
				//それ以外は無効にする。
				else {

					m_triggerBox.Deactivate();
				}
			}
			//攻撃時以外は無効にする。
			else {

				m_triggerBox.Deactivate();
			}

			//トリガーボックスを更新。
			m_triggerBox.Update(pos, rot, forward);
		}
	}
}