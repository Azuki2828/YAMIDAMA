#include "stdafx.h"
#include "BackGround.h"

namespace nsMyGame {

	bool CBackGround::Start() {

		m_modelRender = NewGO<CModelRender>(0);
		m_modelRender->SetFilePathTkm("Assets/modelData/bg/bg.tkm");
		m_modelRender->SetPosition(m_pos);
		m_modelRender->SetRotation(m_rot);
		m_modelRender->SetScale(m_sca);
		m_modelRender->SetShadowReceiverFlag(true);
		m_modelRender->Init();
		m_modelRender->Update();

		m_wall = NewGO<CModelRender>(0);
		m_wall->SetFilePathTkm("Assets/modelData/backGround/wall.tkm");
		m_wall->SetPosition(m_pos);
		m_wall->SetRotation(m_rot);
		m_wall->SetScale(m_sca);
		m_wall->SetShadowReceiverFlag(true);
		m_wall->Init();
		m_wall->Update();


		return true;
	}

	void CBackGround::Update() {

	}
}