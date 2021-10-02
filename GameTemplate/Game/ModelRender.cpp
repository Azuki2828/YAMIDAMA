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

	//シャドウレシーバーフラグがtrueならシャドウレシーバー専用のfxファイルを指定する。
	if (m_shadowReceiverFlag) {
		m_modelInitData.m_fxFilePath = "Assets/shader/shadowReceiverModel.fx";
	}

	//コンスタントバッファにライト情報を入れる。
	m_modelInitData.m_expandConstantBuffer = LightManager().GetInstance()->GetLigData();
	m_modelInitData.m_expandConstantBufferSize = sizeof(*LightManager().GetInstance()->GetLigData());

	//レジスタのt10にシャドウマップを設定。
	m_modelInitData.m_expandShaderResoruceView = &RenderTarget::GetRenderTarget(enShadowMap)->GetRenderTargetTexture();

	//スケルトンを設定。
	if (m_skeleton.IsInited()) {
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
	if (m_shadowCasterFlag) {
		ModelInitData ShadowModelInitData;
		ShadowModelInitData.m_fxFilePath = FX_FILE_PATH_SHADOW_MAP;
		ShadowModelInitData.m_tkmFilePath = m_filePathTkm;
		ShadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		ShadowModelInitData.m_vsEntryPointFunc = VS_ENTRY_POINT_VS_MAIN;
		ShadowModelInitData.m_vsSkinEntryPointFunc = VS_ENTRY_POINT_VS_SKIN_MAIN;

		if (m_skeleton.IsInited()) {
			ShadowModelInitData.m_skeleton = &m_skeleton;
		}
		m_shadowModel.Init(ShadowModelInitData);
		m_shadowModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);
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

	//シャドウモデルの情報を更新。
	m_shadowModel.UpdateWorldMatrix(
		m_pos,
		m_rot,
		m_sca
	);

	//カメラの上方向を求める。
	Vector3 Vec_x = Vector3::AxisX;
	Vector3 TarPos = Camera::GetLightCamera()->GetTarget() - Camera::GetLightCamera()->GetPosition();
	TarPos.Normalize();
	Vector3 CameraUp;

	CameraUp.Cross(TarPos, Vec_x);
	CameraUp.Normalize();
	Camera::GetLightCamera()->SetUp(CameraUp);
	Camera::GetLightCamera()->Update();
}

void ModelRender::Render(RenderContext& rc) {

	switch (rc.GetRenderMode()) {
		case RenderContext::EnRender_Mode::enRenderMode_Normal:
			//モデルを描画。
			m_model.Draw(rc);
			break;
		case RenderContext::EnRender_Mode::enRenderMode_Shadow:
			//シャドウモデルを描画。
			m_shadowModel.Draw(rc, *Camera::GetLightCamera());
			break;
	}
}
