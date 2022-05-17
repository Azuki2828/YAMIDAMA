#include "stdafx.h"
#include "CameraManager.h"

namespace nsMyGame {

	namespace nsCamera {

		bool CCameraManager::Start() {

			//カメラを生成。
			CreateCamera();

			return true;
		}

		void CCameraManager::CreateCamera() {

			//必要なカメラをポインタに格納。
			m_camera[enCamera_Main] = &m_mainCamera;
			m_camera[enCamera_LockOn] = &m_lockOnCamera;

			//それぞれのカメラのStart()関数を呼び出す。
			for (const auto& camera : m_camera) {

				camera->Start();
			}
		}
	}
}