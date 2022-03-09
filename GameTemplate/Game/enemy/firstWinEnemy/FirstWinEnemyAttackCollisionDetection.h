#pragma once
#include "../constEnemy.h"
#include "../../AttackCollision.h"

namespace nsMyGame {

	namespace nsEnemy {
		
		//先手必勝の敵の攻撃処理に使うトリガーボックスのクラス
		class CFirstWinEnemyAttackCollisionDetection
		{
		public:
			/**
			 * @brief トリガーボックスを作成する関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Create(const CVector3& pos, const CQuaternion& rot) {

				m_attackCollision.CreateBox(pos, rot, c_attackTriggerBoxSize);
			}

			/**
			 * @brief トリガーボックスを有効にする関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Activate() {

				m_isActive = true;
			}

			/**
			 * @brief トリガーボックスを無効にする関数。
			*/
			void Deactivate() {

				m_isActive = false;
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

				m_position = position;

				CQuaternion rotation;
				rotation.SetRotation(worldMatrix);
				m_attackCollision.SetRotation(rotation);
			}

			/**
			 * @brief 更新関数。
			 * @param pos 座標
			 * @param rot 回転
			 * @param forward 前方向
			*/
			void Update();

			/**
			 * @brief 攻撃がガードされた？
			 * @return 攻撃がガードされたかどうかのフラグ
			*/
			const bool IsGuarded()const {

				return m_isGuarded;
			}
		private:
			bool m_isActive = false;			//トリガーボックスが有効？
			bool m_isGuarded = false;			//攻撃がガードされた？
			CVector3 m_position = CVector3::Zero;	//座標

			CPhysicsGhostObject m_attackCollision;	//攻撃用当たり判定
		};
	}
}

