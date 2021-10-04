#include "stdafx.h"
#include "ModelRender.h"

namespace NMyRenderer {

	namespace {
		constexpr const char* c_entryPointVSMain = "VSMain";
		constexpr const char* c_entryPointVSSkinMain = "VSSkinMain";
		constexpr const char* c_fxFilePath_Model = "Assets/shader/model.fx";
	}

	void ModelRender::Init() {

		//tkmファイルとtksファイルのパスを設定。
		SetFilePathTkmAndTks();

		//頂点シェーダーのエントリーポイントを指定。
		m_modelInitData.m_vsEntryPointFunc = c_entryPointVSMain;
		m_modelInitData.m_vsSkinEntryPointFunc = c_entryPointVSSkinMain;

		//fxファイルパスを指定。
		m_modelInitData.m_fxFilePath = c_fxFilePath_Model;

		//コンスタントバッファにモデルデータを入れる。
		m_modelInitData.m_expandConstantBuffer = &m_sModelData;
		m_modelInitData.m_expandConstantBufferSize = sizeof(m_sModelData);

		//レジスタのt10にシャドウマップを設定。
		m_modelInitData.m_expandShaderResoruceView = &RenderTarget::GetRenderTarget(enShadowMap)->GetRenderTargetTexture();

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウキャスターフラグがtrueならシャドウモデルを作成する。
		if (m_shadowCasterFlag) { CreateShadowModel(); }

		//モデルデータを元にモデルを初期化。
		m_model.Init(m_modelInitData);

		//アニメーションを初期化。
		m_animation.Init(m_skeleton, m_animationClip, m_animNum);
	}

	void ModelRender::CreateShadowModel() {

		//シャドウモデルのデータを初期化。
		ModelInitData ShadowModelInitData;
		ShadowModelInitData.m_fxFilePath = c_fxFilePath_ShadowMap;
		ShadowModelInitData.m_tkmFilePath = m_filePathTkm;
		ShadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		ShadowModelInitData.m_vsEntryPointFunc = c_entryPointVSMain;
		ShadowModelInitData.m_vsSkinEntryPointFunc = c_entryPointVSSkinMain;

		//スケルトンを設定。
		if (m_skeleton.IsInited()) {
			ShadowModelInitData.m_skeleton = &m_skeleton;
		}
		//シャドウモデルを初期化。
		m_shadowModel.Init(ShadowModelInitData);

		//シャドウモデルの座標、回転率、拡大率を更新。
		m_shadowModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);
	}

	void ModelRender::SetFilePathTkmAndTks() {

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
	}

	void ModelRender::Update() {

		//スケルトンを更新。
		m_skeleton.Update(m_model.GetWorldMatrix());

		//アニメーションを進める。
		if (m_animFlg) {
			m_animation.Progress(GameTime().GameTimeFunc().GetFrameDeltaTime());
		}

		//モデルの座標、回転率、拡大率を更新。
		m_model.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);

		//シャドウモデルの座標、回転率、拡大率を更新。
		m_shadowModel.UpdateWorldMatrix(
			m_pos,
			m_rot,
			m_sca
		);
	}

	void ModelRender::Render(RenderContext& rc) {

		//描画モードに応じて描画するモデルを変える。
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
}
