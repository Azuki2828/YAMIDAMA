#pragma once

class CPlayer;

class CMainCamera : public CIGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	CVector3 m_pos = {0.0f,50.0f,200.0f};
	CQuaternion m_rot = CQuaternion::Identity;
	CVector3 m_sca = CVector3::One;

	CVector3 m_tar = {0.0f,50.0f,0.0f};
	CVector3 toPos = { 0.0f,0.0f,200.0f };

	CPlayer* m_player = nullptr;
};

