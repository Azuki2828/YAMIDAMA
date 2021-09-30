#include "stdafx.h"
#include "ModelRender.h"

namespace {
	constexpr const char* VS_ENTRY_POINT_VS_MAIN = "VSMain";
	constexpr const char* VS_ENTRY_POINT_VS_SKIN_MAIN = "VSSkinMain";
	constexpr const char* FX_FILE_PATH_MODEL = "Assets/shader/model.fx";
}

void ModelRender::Init() {

	//tkmファイルのロード。
	if (m_filePathTkm != nullptr) {
		m_modelInitData.m_tkmFilePath = m_filePathTkm;
	}
	else {
		//ロードエラー。
		MessageBox(nullptr, L"tkmファイルが読み込まれていません。", L"警告", MB_OK);
	}

	//tksファイルのロード。
	if (m_filePathTks != nullptr) {
		m_skeleton.Init(m_filePathTks);
	}

	//頂点シェーダーのエントリーポイントを指定。
	m_modelInitData.m_vsEntryPointFunc = VS_ENTRY_POINT_VS_MAIN;
	m_modelInitData.m_vsSkinEntryPointFunc = VS_ENTRY_POINT_VS_SKIN_MAIN;

	//fxファイルパスを指定。
	m_modelInitData.m_fxFilePath = FX_FILE_PATH_MODEL;

	//スケルトンを設定。
	if (m_skeleton.IsInited()) {
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	//モデルデータを元にモデルを初期化。
	m_model.Init(m_modelInitData);

	//アニメーションを初期化。
	m_animation.Init(m_skeleton, m_animationClip, m_animNum);
}

void ModelRender::Update() {

	//スケルトンを更新。
	m_skeleton.Update(m_model.GetWorldMatrix());

	//アニメーションを進める。
	if (m_animFlg) {
		m_animation.Progress(GameTime().GameTimeFunc().GetFrameDeltaTime());
	}

	//モデルの情報を更新。
	m_model.UpdateWorldMatrix(
		m_pos,
		m_rot,
		m_sca
	);
}

void ModelRender::Render(RenderContext& rc) {

	//モデルを描画。
	m_model.Draw(rc);
}
