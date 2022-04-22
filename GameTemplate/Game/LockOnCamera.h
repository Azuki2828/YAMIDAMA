#pragma once
#include "../../MiniEngine/SpringCamera.h"
#include "enemy/Enemy.h"
#include "CameraBase.h"

namespace nsMyGame {

	//ロックオンカメラクラス
	class CLockOnCamera : public CCameraBase
	{
	private:
		/**
		 * @brief 初期化関数。
		 * @return 成功した？
		*/
		bool StartSub()override;

		/**
		 * @brief 更新関数。
		*/
		void UpdateSub()override;

		/**
		 * @brief 敵をロックオンする関数。
		*/
		void LockOnEnemy();

		
	public:
		/**
		 * @brief ロックオン中の敵の座標を取得する関数。
		 * @return ロックオン中の敵の座標
		*/
		const CVector3& GetLockOnEnemyPosition()const override final {

			return m_lockOnEnemy->GetPosition();
		}
	private:
		SpringCamera m_springCamera;				//ばねカメラ
		CVector3 m_position = CVector3::Zero;		//視点
		CVector3 m_target = CVector3::Zero;			//注視点
		nsEnemy::CEnemy* m_lockOnEnemy = nullptr;	//ロックオン対象の敵
	};
}

