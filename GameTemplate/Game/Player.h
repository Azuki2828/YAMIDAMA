#pragma once

class Player : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	ModelRender* m_modelRender = nullptr;
};

