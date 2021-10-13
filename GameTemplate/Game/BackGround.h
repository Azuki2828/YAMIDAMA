#pragma once

namespace nsMyGame {

	class CBackGround : public IGameObject
	{
	public:
		bool Start()override final;
		void Update()override final;

	private:
		CModelRender* m_modelRender = nullptr;
		CModelRender* m_wall = nullptr;

		Vector3 m_pos = Vector3::Zero;
		Quaternion m_rot = Quaternion::Identity;
		Vector3 m_sca = Vector3::One;
	};
}

