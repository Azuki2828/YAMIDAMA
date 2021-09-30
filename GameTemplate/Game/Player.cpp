#include "stdafx.h"
#include "Player.h"

bool Player::Start() {

	m_modelRender = NewGO<ModelRender>(0);
	m_modelRender->SetFilePathTkm("Assets/modelData/unityChan.tkm");
	m_modelRender->SetFilePathTks("Assets/modelData/unityChan.tks");
	m_animationClip.Load("Assets/animData/idle.tka");
	m_animationClip.SetLoopFlag(true);
	m_modelRender->InitAnimation(&m_animationClip, 1);
	m_modelRender->SetAnimFlg(true);
	m_modelRender->Init();
	return true;
}

void Player::Update() {

	m_modelRender->PlayAnimation(0);
}