#pragma once
#include "../../AttackCollision.h"

namespace nsMyGame {

	namespace nsEnemy {

		//先手必勝の敵の攻撃処理に使うトリガーボックスのクラス
		class CBossCollisionDetection
		{
		public:
			/**
			 * @brief トリガーボックスを作成する関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Create(const CVector3& pos, const CQuaternion& rot) {

				m_attackCollision.CreateBox(pos, rot, c_bossAttackTriggerBoxSize);
				m_jumpAttackCollision.CreateBox(pos, rot, { 500.0f,100.0f,500.0f });
			}

			/**
			 * @brief トリガーボックスを有効にする関数。
			*/
			void ActivateAttack() {

				m_isAttackActive = true;
			}

			/**
			 * @brief トリガーボックスを無効にする関数。
			*/
			void DeactivateAttack() {

				m_isAttackActive = false;
			}

			/**
			 * @brief トリガーボックスを有効にする関数。
			*/
			void ActivateRangeAttack() {

				m_isRangeAttackActive = true;
			}

			/**
			 * @brief トリガーボックスを無効にする関数。
			*/
			void DeactivateRangeAttack() {

				m_isRangeAttackActive = false;
			}

			/**
			 * @brief 座標と回転を更新する関数。
			 * @param worldMatrix ワールド行列
			*/
			void UpdatePositionAndRotation(const CMatrix& worldMatrix) {

				CVector3 position;
				position.x = worldMatrix.m[3][0];
				position.y = worldMatrix.m[3][1];
				position.z = worldMatrix.m[3][2];
				m_attackCollision.SetPosition(position);

				CQuaternion rotation;
				rotation.SetRotation(worldMatrix);
				m_attackCollision.SetRotation(rotation);
			}

			/**
			 * @brief 更新関数。
			 * @param pos 座標
			*/
			void Update(const CVector3& pos);

			/**
			 * @brief 攻撃がガードされた？
			 * @return 攻撃がガードされたかどうかのフラグ
			*/
			const bool IsGuarded()const {

				return m_isGuarded;
			}
		private:
			bool m_isAttackActive = false;			//トリガーボックスが有効？
			bool m_isRangeAttackActive = false;			//トリガーボックスが有効？
			bool m_isGuarded = false;			//攻撃がガードされた？

			CPhysicsGhostObject m_attackCollision;	//攻撃用当たり判定
			CPhysicsGhostObject m_jumpAttackCollision;	//攻撃用当たり判定
		};
	}
}

