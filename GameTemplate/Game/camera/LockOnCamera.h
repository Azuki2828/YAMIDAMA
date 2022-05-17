#pragma once
#include "../../MiniEngine/SpringCamera.h"
#include "../enemy/Enemy.h"
#include "CameraBase.h"
#include "LockOnMarker.h"

namespace nsMyGame {

	namespace nsCamera {

		//ロックオンカメラクラス
		class CLockOnCamera : public CCameraBase
		{
			//右スティックの入力情報
			enum R3ButtonInput {

				No_Input,		//入力なし
				Input_Right,	//右
				Input_Left		//左
			};
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

			void SubRelease()override final {

				//ロックオンアイコンを削除。
				m_lockOnMarker.Release();
			}

			/**
			 * @brief 敵をロックオンする関数。
			*/
			const bool LockOnEnemy(R3ButtonInput r3Input = No_Input);

			/**
			 * @brief カメラが切り替わった。
			*/
			void SubSwitched()override final {

				//ロックオンアイコンを初期化。
				m_lockOnMarker.Init();

				//カメラのベクトルを初期化。
				InitCameraToPlayerVec();
			}

			/**
			 * @brief カメラのベクトルを初期化する関数。
			*/
			void InitCameraToPlayerVec();

			/**
			 * @brief 更新関数。
			*/
			void UpdateLockOnCamera();

			/**
			 * @brief ロックオンアイコンの更新関数。
			*/
			void UpdateLockOnMarker();

			/**
			 * @brief カメラの切り替えが可能か？
			 * @return 敵を感知できた？
			*/
			const bool CanSwitch()override final {

				return LockOnEnemy();
			}

			/**
			 * @brief ロックオン対象を切り替える関数。
			*/
			void ChangeLockOnEnemy();

			/**
			 * @brief 右スティックが入力されているか調べる関数。
			 * @return 右スティックが入力されている？
			*/
			const bool CheckR3Input();
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
			nsEnemy::CEnemy* m_lockOnEnemy = nullptr;	//ロックオン対象の敵
			CLockOnMarker m_lockOnMarker;				//ロックオンアイコン
			bool m_isR3XInput = false;					//右スティック入力されている？
			R3ButtonInput m_r3Input = No_Input;			//右スティックの入力情報
		};
	}
}

