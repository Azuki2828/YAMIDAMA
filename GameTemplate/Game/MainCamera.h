#pragma once


namespace nsMyGame {

	namespace nsPlayer {

		class CPlayer;
	}

	class CMainCamera : public CIGameObject
	{
	public:
		bool Start()override final;
		void Update()override final;
	private:
		CVector3 m_pos = CVector3::Zero;
		CQuaternion m_rot = CQuaternion::Identity;
		CVector3 m_sca = CVector3::One;

		CVector3 m_tar = CVector3::Zero;
		CVector3 toPos = { 0.0f,200.0f,-200.0f };

		nsPlayer::CPlayer* m_player = nullptr;
	};
}

