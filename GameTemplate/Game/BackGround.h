#pragma once
#include "../../ExEngine/physics/PhysicsStaticObject.h"

namespace nsMyGame {

	class CBackGround : public CIGameObject
	{
	public:
		bool Start()override final;
		void Update()override final;

	private:
		CModelRender* m_modelRender = nullptr;
		CModelRender* m_wall = nullptr;

		CVector3 m_pos = CVector3::Zero;
		CQuaternion m_rot = CQuaternion::Identity;
		CVector3 m_sca = CVector3::One;
		Level m_level;
		PhysicsStaticObject m_physicsStaticObject;
	};
}

