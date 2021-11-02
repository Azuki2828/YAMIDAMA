#pragma once

namespace nsMyGame {

	namespace nsEnemy {

		namespace nsPlayer {

			class CPlayer;
		}
		//先手必勝の敵の攻撃処理に使うトリガーボックスのクラス
		class CFirstWinEnemyTriggerBox
		{
		private:

			void Create(const CVector3& pos, const CQuaternion& rot);
		public:

			void Activate(const CVector3& pos, const CQuaternion& rot);

			void Deactivate();

			void Update(const CVector3& pos, const CQuaternion& rot, const CVector3& forward);

		private:
			bool m_isActive = false;			//トリガーボックスが有効かどうか？

			CVector3 m_position;				//座標
			CQuaternion m_rotation;				//回転
			CVector3 m_forward;					//前方向
			CPhysicsGhostObject m_ghostBox;		//トリガーボックス

			nsPlayer::CPlayer* m_player = nullptr;
		};
	}
}

