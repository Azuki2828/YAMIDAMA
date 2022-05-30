#pragma once
#include "MainCamera.h"
#include "LockOnCamera.h"

namespace nsMyGame {

	namespace nsCamera {

		//カメラの種類
		enum EnCameraType {
			enCamera_Main,			//通常カメラ
			enCamera_LockOn,		//ロックオンカメラ

			enCamera_Num			//カメラの数
		};

		//カメラ管理クラス
		class CCameraManager : public CIGameObject
		{
		private:
			/**
			 * @brief 初期化関数。
			 * @return 成功した？
			*/
			bool Start()override final;

			/**
			 * @brief 削除関数。
			*/
			void OnDestroy()override final {

				//カメラを解放。
				Release();
			}

			/**
			 * @brief 更新関数。
			*/
			void Update()override final {

				//現在のカメラが無効になったらメインカメラに戻す。
				if (!m_camera[m_cameraType]->IsEnable()) {

					SetCameraType(enCamera_Main);
				}

				//カメラを更新。
				m_camera[m_cameraType]->Update();
			}

			/**
			 * @brief カメラを生成する関数。
			*/
			void CreateCamera();
		public:

			/**
			 * @brief 解放関数。
			*/
			void Release() {

				//各カメラの解放を行う。
				for (const auto& camera : m_camera) {

					camera->Release();
				}
			}

			/**
			 * @brief カメラの視点の座標を取得する関数。
			 * @return カメラの視点の座標
			*/
			const CVector3& GetPosition()const {

				return m_camera[m_cameraType]->GetPosition();
			}

			/**
			 * @brief カメラの注視点の座標を取得する関数。
			 * @return カメラの注視点の座標
			*/
			const CVector3& GetTarget()const {

				return m_camera[m_cameraType]->GetTarget();
			}

			/**
			 * @brief 切り替え可能か？
			 * @return 成功した？
			*/
			const bool CanSwitch(EnCameraType cameraType) {

				return m_camera[cameraType]->CanSwitch();
			}

			/**
			 * @brief カメラの種類を設定する関数。
			 * @param cameraType カメラの種類
			*/
			void SetCameraType(EnCameraType cameraType) {

				//先にすべてのカメラを解放。
				for (const auto& camera : m_camera) { camera->Release(); }

				//カメラタイプを設定。
				m_cameraType = cameraType;

				//カメラが切り替わったことを知らせる。
				m_camera[m_cameraType]->Switched();
			}

			/**
			 * @brief カメラの種類を主六する関数。
			 * @return カメラの種類
			*/
			const EnCameraType GetCameraType()const {

				return m_cameraType;
			}

			/**
			 * @brief ロックオン中の敵の座標を取得する関数。
			 * @return ロックオン中の敵の座標
			*/
			const CVector3& GetLockOnEnemyPosition() {

				return m_lockOnCamera.GetLockOnEnemyPosition();
			}

			/**
			 * @brief カメラを揺れ状態にする関数。
			*/
			void ShakeCamera() {

				m_camera[m_cameraType]->ShakeCamera();
			}

			/**
			 * @brief カメラの状態を通常状態にする関数。
			*/
			void SetNormalCamera() {

				m_camera[m_cameraType]->SetNormalCamera();
			}
		private:
			EnCameraType m_cameraType = enCamera_Main;				//カメラの種類
			CCameraBase* m_camera[enCamera_Num] = { nullptr };		//カメラのポインタ配列

			CMainCamera m_mainCamera;								//メインカメラ
			CLockOnCamera m_lockOnCamera;							//ロックオンカメラ
		};
	}
}

