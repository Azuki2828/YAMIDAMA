#include "stdafx.h"
#include "Enemy.h"
#include "../AttackCollision.h"

namespace nsMyGame {

	namespace nsEnemy {

		CEnemy* g_pCurrentEnemy = nullptr;

		/**
		 * @brief ステートを変更する関数。
		 * @param stateNum ステート番号
		*/
		void ChangeState(int stateNum) {

			g_pCurrentEnemy->ChangeState(stateNum);
		}

		/**
		 * @brief プレイヤーとの距離を測る関数。
		 * @return プレイヤーとの距離
		*/
		float GetLengthToPlayer() {

			return g_pCurrentEnemy->GetLengthToPlayer();
		}

		/**
		 * @brief 移動関数。
		*/
		void Move() {

			g_pCurrentEnemy->Move();
		}

		/**
		 * @brief 回転関数。
		*/
		void Rotate() {

			g_pCurrentEnemy->Rotate();
		}

		/**
		 * @brief クールタイムを設定する関数。
		 * @param coolTime クールタイム
		*/
		void SetCoolTime(const float coolTime) {

			g_pCurrentEnemy->SetCoolTime(coolTime);
		}

		/**
		 * @brief クールタイムを取得する関数。
		 * @return クールタイム
		*/
		float GetCoolTime() {

			return g_pCurrentEnemy->GetCoolTime();
		}

		/**
		 * @brief アニメーションを更新する関数。
		*/
		void AnimationUpdate() {

			g_pCurrentEnemy->AnimationUpdate();
		}

		/**
		 * @brief 死んでいる？
		 * @return 死んでいるかどうかのフラグ
		*/
		bool IsDeath() {

			return g_pCurrentEnemy->IsDeath();
		}

		/**
		 * @brief 自身を削除する関数。
		*/
		void Delete() {

			g_pCurrentEnemy->Delete();
		}

		/**
		 * @brief トリガーボックスを更新する関数。
		*/
		void UpdateTriggerBox() {

			g_pCurrentEnemy->UpdateTriggerBox();
		}

		void SetGuardTime(const float guardTime) {

			g_pCurrentEnemy->SetGuardTime(guardTime);
		}

		float GetGuardTime() {

			return g_pCurrentEnemy->GetGuardTime();
		}

		bool IsGuarded() {

			return g_pCurrentEnemy->IsGuarded();
		}

		void JudgeDamage() {

			g_pCurrentEnemy->JudgeDamage();
		}

		bool NoticePlayer() {

			return g_pCurrentEnemy->NoticePlayer();
		}

		//Python側に関数を渡す。
		PYBIND11_MODULE(Game, m) {
			m.def("ChangeState", &ChangeState);
			m.def("GetLengthToPlayer", &GetLengthToPlayer);
			m.def("Move", &Move);
			m.def("Rotate", &Rotate);
			m.def("SetCoolTime", &SetCoolTime);
			m.def("GetCoolTime", &GetCoolTime);
			m.def("AnimationUpdate", &AnimationUpdate);
			m.def("IsDeath", &IsDeath);
			m.def("Delete", &Delete);
			m.def("UpdateTriggerBox", &UpdateTriggerBox);
			m.def("SetGuardTime", &SetGuardTime);
			m.def("GetGuardTime", &GetGuardTime);
			m.def("IsGuarded", &IsGuarded);
			m.def("JudgeDamage", &JudgeDamage);
			m.def("NoticePlayer", &NoticePlayer);
		}


		bool CEnemy::Start()
		{
			//プレイヤーを検索。
			m_player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//派生クラスのStartSub()関数の結果を返す。
			return StartSub();
		}

		void CEnemy::OnDestroy() {

			//モデルを削除。
			if (m_modelRender != nullptr) {
				DeleteGO(m_modelRender);
			}
		}

		void CEnemy::Update()
		{
			//派生クラスのUpdateSub()関数を呼び出す。
			UpdateSub();

			//前方向を更新。
			UpdateForward();

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//クールタイムを更新。
			if (m_coolTime > 0.0f) {

				m_coolTime -= g_gameTime->GetFrameDeltaTime();
			}
			else {
				m_coolTime = 0.0f;
			}

			//ガード時間を更新。
			if (m_guardTime > 0.0f) {

				m_guardTime -= g_gameTime->GetFrameDeltaTime();
			}
			else {

				m_guardTime = 0.0f;
			}
		}
		void CEnemy::Render(CRenderContext& rc)
		{

		}

		void CEnemy::Rotate() {

			//移動ボタンが入力されていなかったら
			if (fabsf(m_moveSpeed.x) < 0.001f
				&& fabsf(m_moveSpeed.z) < 0.001f) {
				//このフレームではキャラは移動していないので旋回する必要はない。
				return;
			}

			//回転角度を求める。
			float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);

			//回転を設定。
			m_rotation.SetRotationY(-angle);
		}

		void CEnemy::UpdateForward() {

			//回転行列を計算。
			auto mRot = CMatrix::Identity;
			mRot.MakeRotationFromQuaternion(m_rotation);

			//前方向を設定。
			m_forward.x = mRot.m[2][0];
			m_forward.y = mRot.m[2][1];
			m_forward.z = mRot.m[2][2];

			//正規化。
			m_forward.Normalize();
		}

		void CEnemy::JudgeDamage() {

			//生成されている敵の攻撃当たり判定を調べる。
			auto playerCollision = FindGOs<CAttackCollision>(c_classNamePlayerAttackCollision);

			//当たり判定処理。
			//このフレームで全ての当たり判定との衝突判定をするため、一気にダメージを受ける可能性あり。
			//それが嫌なら、for文の内部でステートを調べること！
			for (auto& collision : playerCollision) {

				//ガード成功中なら早期リターン。
				//if (m_playerState == enState_GuardSuccess) {
				//
				//	return;
				//}

				//剛体との当たり判定を調べる。
				CPhysicsWorld::GetInstance()->ContactTest(m_charaCon, [&](const btCollisionObject& contactObject) {

					//トリガーボックスと接触した。
					if (collision->IsSelf(contactObject)) {

						////ガードしたならガード成功状態に。
						//if (m_playerState == enState_Guard) {
						//
						//	m_playerState = enState_GuardSuccess;
						//	m_playerAction.GuardSuccess();
						//
						//	//処理はここで終了。
						//	return;
						//}

						//ダメージを受ける。
						SetReceiveDamage(true);
					}
					});
			}
		}
	}
}