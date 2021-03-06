#pragma once
#include "../constEnemy.h"

namespace nsMyGame {

	namespace nsPlayer {

		class CPlayer;
	}

	namespace nsEnemy {


		//先手必勝の敵の攻撃処理に使うトリガーボックスのクラス
		class CGoteWinEnemyCollisionDetection
		{
		private:
			/**
			 * @brief トリガーボックスを作成する関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Create(const CVector3& pos, const CQuaternion& rot);
		public:

			/**
			 * @brief トリガーボックスを有効にする関数。
			 * @param pos 座標
			 * @param rot 回転
			*/
			void Activate(const CVector3& pos, const CQuaternion& rot);

			/**
			 * @brief トリガーボックスを無効にする関数。
			*/
			void Deactivate();

			/**
			 * @brief 更新関数。
			 * @param pos 座標
			 * @param rot 回転
			 * @param forward 前方向
			*/
			void Update(const CVector3& pos, const CQuaternion& rot, const CVector3& forward);

			/**
			 * @brief 攻撃がガードされた？
			 * @return 攻撃がガードされたかどうかのフラグ
			*/
			bool IsGuarded() {

				return m_isGuarded;
			}
		private:
			bool m_isActive = false;			//トリガーボックスが有効？
			bool m_isGuarded = false;			//攻撃がガードされた？

			CVector3 m_position;				//座標
			CQuaternion m_rotation;				//回転
			CVector3 m_forward;					//前方向
			CPhysicsGhostObject m_triggerBox;		//トリガーボックス

			nsPlayer::CPlayer* m_player = nullptr;
		};
	}
}

