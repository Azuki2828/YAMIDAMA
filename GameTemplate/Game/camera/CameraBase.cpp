#include "stdafx.h"
#include "CameraBase.h"
#include "constCamera.h"

namespace nsMyGame {

	namespace nsCamera {

		CVector3 CCameraBase::m_cameraToPlayerVec = c_defaultCameraVec;
		EnState CCameraBase::m_state = enNormal;
	}
}
