#pragma once

class Player : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

	Vector3 GetPosition() {

		return m_pos;
	}

private:
	ModelRender* m_modelRender = nullptr;
	AnimationClip m_animationClip;

	Vector3 m_pos = Vector3::Zero;
};

