#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start() {

	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->SetFilePathTkm("Assets/modelData/bg/bg.tkm");
	m_modelRender->SetPosition(m_pos);
	m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_sca);
	m_modelRender->Init();
	m_modelRender->Update();
	return true;
}

void BackGround::Update() {

}