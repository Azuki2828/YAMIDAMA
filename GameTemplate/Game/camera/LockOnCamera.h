#pragma once
#include "../../MiniEngine/SpringCamera.h"
#include "../enemy/Enemy.h"
#include "CameraBase.h"
#include "LockOnMarker.h"

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

		void Release()override final {

			//ロックオンアイコンを削除。
			m_lockOnMarker.Release();
		}

		/**
		 * @brief 敵をロックオンする関数。
		*/
		const bool LockOnEnemy();

		/**
		 * @brief カメラが切り替わった。
		*/
		void Switched()override final {

			//ロックオンアイコンを初期化。
			m_lockOnMarker.Init();
		}

		/**
		 * @brief カメラの切り替えが可能か？
		 * @return 敵を感知できた？
		*/
		const bool CanSwitch()override final {

			return LockOnEnemy();
		}
	public:
		/**
		 * @brief ロックオン中の敵の座標を取得する関数。
		 * @return ロックオン中の敵の座標
		*/
		const CVector3& GetLockOnEnemyPosition()const {

			return m_lockOnEnemy->GetPosition();
		}
	private:
		SpringCamera m_springCamera;				//ばねカメラ
		CVector3 m_position = CVector3::Zero;		//視点
		CVector3 m_target = CVector3::Zero;			//注視点
		nsEnemy::CEnemy* m_lockOnEnemy = nullptr;	//ロックオン対象の敵
		CLockOnMarker m_lockOnMarker;				//ロックオンアイコン
		CVector3 LockOnIconPosition;
	};
}

