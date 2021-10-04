#pragma once
class BackGround : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	NMyRenderer::ModelRender* m_modelRender = nullptr;

	Vector3 m_pos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_sca = Vector3::One;
};

