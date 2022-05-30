#pragma once

namespace nsMyGame {

	namespace nsCamera {

		//ステート
		enum EnState {
			enNormal,		//通常
			enShake			//揺れ状態
		};

		const CVector3 c_defaultCameraVec = { 0.0f,150.0f,200.0f };
		constexpr float c_addCameraTarget = 110.0f;			//カメラの注視点を決める値
		constexpr float c_cameraFar = 80000.0f;				//カメラの遠平面
		constexpr float c_cameraCollisionRadius = 5.0f;	//コリジョンの半径
		constexpr int c_cameraShakeRange = 201;
		constexpr float c_cameraShakeRangeAdjustment = 100.0f;
	}
}