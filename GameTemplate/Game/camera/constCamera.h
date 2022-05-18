#pragma once

namespace nsMyGame {

	namespace nsCamera {

		const CVector3 c_defaultCameraVec = { 0.0f,150.0f,200.0f };
		constexpr float c_addCameraTarget = 80.0f;			//カメラの注視点を決める値
		constexpr float c_cameraFar = 80000.0f;				//カメラの遠平面
		constexpr float c_cameraCollisionRadius = 5.0f;	//コリジョンの半径
	}
}