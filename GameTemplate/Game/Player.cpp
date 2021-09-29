#include "stdafx.h"
#include "Player.h"

bool Player::Start() {

	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->SetFilePathTkm("Assets/modelData/unityChan.tkm");
	m_modelRender->SetFilePathTks("Assets/modelData/unityChan.tks");
	m_modelRender->Init();
	return true;
}

void Player::Update() {

}