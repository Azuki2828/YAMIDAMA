#include "stdafx.h"
#include "ModelRender.h"

namespace {
	constexpr const char* VS_ENTRY_POINT_VS_MAIN = "VSMain";
	constexpr const char* VS_ENTRY_POINT_VS_SKIN_MAIN = "VSSkinMain";
	constexpr const char* FX_FILE_PATH_MODEL = "Assets/shader/model.fx";
}

void ModelRender::Init() {

	//���j�e�B�[�����̃A�j���[�V�����ɍ��킹�邽�߁A����������YUp�ɂ��Ă����B
	m_modelInitData.m_modelUpAxis = enModelUpAxisY;


	//tkm�t�@�C���̃��[�h�B
	if (m_filePathTkm != nullptr) {
		m_modelInitData.m_tkmFilePath = m_filePathTkm;
	}
	else {
		//���[�h�G���[�B
		MessageBox(nullptr, L"tkm�t�@�C�����ǂݍ��܂�Ă��܂���B", L"�x��", MB_OK);
	}

	//tks�t�@�C���̃��[�h�B
	if (m_filePathTks != nullptr) {
		m_skeleton.Init(m_filePathTks);
	}
	else {
		//���[�h�G���[�B
		MessageBox(nullptr, L"tks�t�@�C�����ǂݍ��܂�Ă��܂���B", L"�x��", MB_OK);
	}

	//���_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
	m_modelInitData.m_vsEntryPointFunc = VS_ENTRY_POINT_VS_MAIN;
	m_modelInitData.m_vsSkinEntryPointFunc = VS_ENTRY_POINT_VS_SKIN_MAIN;

	//fx�t�@�C���p�X���w��B
	m_modelInitData.m_fxFilePath = FX_FILE_PATH_MODEL;

	//�X�P���g����ݒ�B
	if (m_skeleton.IsInited()) {
		m_modelInitData.m_skeleton = &m_skeleton;
	}
	else {
		//�G���[�B
		MessageBox(nullptr, L"�X�P���g�����ǂݍ��܂�Ă��܂���B", L"�x��", MB_OK);
	}

	//���f���f�[�^�����Ƀ��f�����������B
	m_model.Init(m_modelInitData);

	m_animation.Init(m_skeleton, m_animationClip, m_animNum);
}

void ModelRender::Update() {

	//�X�P���g�����X�V�B
	m_skeleton.Update(m_model.GetWorldMatrix());

	//�A�j���[�V������i�߂�B
	if (m_animFlg) {
		m_animation.Progress(GameTime().GameTimeFunc().GetFrameDeltaTime());
	}

	//���f���̏����X�V�B
	m_model.UpdateWorldMatrix(
		m_pos,
		m_rot,
		m_sca
	);
}

void ModelRender::Render(RenderContext& rc) {

	//���f����`��B
	m_model.Draw(rc);
}