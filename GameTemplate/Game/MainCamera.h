#pragma once

class Player;

class MainCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	Vector3 m_pos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_sca = Vector3::One;

	Vector3 m_tar = Vector3::Zero;
	Vector3 toPos = { 0.0f,0.0f,200.0f };

	Player* m_player = nullptr;
};

