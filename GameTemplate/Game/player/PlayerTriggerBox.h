#pragma once
namespace nsMyGame {

	namespace nsPlayer {

		//プレイヤーの攻撃処理に使うトリガーボックスのクラス
		class CPlayerTriggerBox
		{
		public:
			/**
			 * @brief 攻撃の当たり判定を作成する関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Create(const CVector3& pos, const CQuaternion& rot) {

				m_attackCollision.CreateBox(pos, rot, c_attackTriggerBoxSize);
			}
			
			/**
			 * @brief トリガーボックスを有効にする関数。
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

				CQuaternion rotation;
				rotation.SetRotation(worldMatrix);
				m_attackCollision.SetRotation(rotation);
			}

			/**
			 * @brief トリガーボックスを更新する関数。
			*/
			void Update();

		private:
			bool m_isActive = false;			//トリガーボックスが有効かどうか？
			CPhysicsGhostObject m_attackCollision;		//トリガーボックス
		};
	}
}

