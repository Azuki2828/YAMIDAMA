#include "stdafx.h"
#include "RenderingEngine.h"

namespace nsMyGame {

	//レンダリングエンジンのインスタンス
	CRenderingEngine* CRenderingEngine::m_renderingEngine = nullptr;

	void CRenderingEngine::Init() {

		//レンダリングターゲットを作成。
		CreateRenderTarget();

		//ポストエフェクトを初期化。
		m_postEffect.Init();

		//ライトカリングの初期化。
		m_lightCulling.Init();

		//ディファードライティング用のスプライトを初期化。
		InitDeferredLightingSprite();

		//メインレンダリングターゲットのスプライトを初期化。
		InitCopyToMainRenderTargetSprite();
	}

	void CRenderingEngine::Render() {

		//レンダーコンテキストを取得。
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		//シャドウマップを描画。
		DrawShadowMap(renderContext);

		//G-Bufferを作成。
		CreateGBuffer(renderContext);

		//ライトカリング。
		m_lightCulling.Execute(renderContext);

		//ディファードライティング(TBDR)。
		ExecuteDeferredLighting(renderContext);

		//メインレンダリングターゲットの絵をスナップショット。
		SnapShotMainRenderTarget(renderContext);

		//ポストエフェクト。
		m_postEffect.Render(renderContext);

		//エフェクトモデルを描画。
		DrawEffect(renderContext);

		//フォントを描画。
		DrawFont(renderContext);

		//ワイヤーフレームを描画。
		DrawWireFrame(renderContext);

		//スプライトを描画。
		RenderSprite(renderContext);

		//フレームバッファを描画。
		CopyToFrameBuffer(renderContext);
	}

	void CRenderingEngine::CreateRenderTarget() {

		//レンダリングターゲットを作成。
		CRenderTarget::CreateMainRenderTarget();
		CRenderTarget::CreateShadowMap();
		CRenderTarget::CreateAlbedoAndShadowReceiverFlagRenderTarget();
		CRenderTarget::CreateNormalMapRenderTarget();
		CRenderTarget::CreateWorldPosRenderTarget();
		CRenderTarget::CreateSpecularRenderTarget();
		CRenderTarget::CreateDepthRenderTarget();
		CreateSnapShotMainRT();
	}

	void CRenderingEngine::InitDeferredLightingSprite() {

		//ディファードレンダリングに必要なデータを設定する。
		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &CRenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[1] = &CRenderTarget::GetGBufferRT(enNormalMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[2] = &CRenderTarget::GetGBufferRT(enWorldPosMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[3] = &CRenderTarget::GetGBufferRT(enDepthMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[4] = &CRenderTarget::GetGBufferRT(enocclusionAndSmoothAndMetaricMap)->GetRenderTargetTexture();
		spriteInitData.m_width = c_renderTargetW1280H720.x;
		spriteInitData.m_height = c_renderTargetW1280H720.y;
		spriteInitData.m_fxFilePath = c_fxFilePath_DeferredLighting;
		spriteInitData.m_expandConstantBuffer = nsLight::CLightManager::GetInstance()->GetLigData();
		spriteInitData.m_expandConstantBufferSize = sizeof(*nsLight::CLightManager::GetInstance()->GetLigData());
		spriteInitData.m_expandShaderResoruceView = &CRenderTarget::GetRenderTarget(enShadowMap)->GetRenderTargetTexture();
		spriteInitData.m_expandShaderResoruceView2 = &m_lightCulling.GetPointLightNoListInTileUAV();

		// 初期化オブジェクトを使って、スプライトを初期化する
		m_deferredLightingSprite.Init(spriteInitData);
	}


	void CRenderingEngine::CreateSnapShotMainRT() {

		//メインレンダリングターゲットの内容をコピーするレンダリングターゲットを作成。
		m_snapShotMainRT.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);
	}
	void CRenderingEngine::InitCopyToMainRenderTargetSprite() {

		SpriteInitData copyToMainRenderTargetSpriteInitData;

		//テクスチャにはメインレンダリングターゲットのテクスチャカラーを指定。
		copyToMainRenderTargetSpriteInitData.m_textures[0] = &CRenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();
		copyToMainRenderTargetSpriteInitData.m_width = c_renderTargetW1280H720.x;
		copyToMainRenderTargetSpriteInitData.m_height = c_renderTargetW1280H720.y;
		copyToMainRenderTargetSpriteInitData.m_fxFilePath = c_fxFilePath_Sprite;
		copyToMainRenderTargetSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_copyToMainRenderTargetSprite.Init(copyToMainRenderTargetSpriteInitData);
	}

	void CRenderingEngine::DrawShadowMap(CRenderContext& rc) {

		//描画モードをシャドウマップ用に設定する。
		rc.SetRenderMode(CRenderContext::EnRender_Mode::enRenderMode_Shadow);

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enShadowMap));

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enShadowMap));

		//レンダーターゲットをクリア。
		rc.ClearRenderTargetView(*CRenderTarget::GetRenderTarget(enShadowMap));

		//シャドウモデルを描画。
		CGameObjectManager::GetInstance()->ExecuteRender(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enShadowMap));
	}

	void CRenderingEngine::RenderSprite(CRenderContext& rc) {

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enMainRT));

		//2Dモデルを描画。
		CGameObjectManager::GetInstance()->Execute2DRender(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
	}

	void CRenderingEngine::DrawFont(CRenderContext& rc) {

		//レンダーモードをフォント用にする。
		rc.SetRenderMode(CRenderContext::EnRender_Mode::enRenderMode_Font);

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enMainRT));

		//フォントを描画。
		CGameObjectManager::GetInstance()->ExecuteRender(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
	}

	void CRenderingEngine::DrawEffect(CRenderContext& rc) {

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
		
		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTarget(
			CRenderTarget::GetRenderTarget(enMainRT)->GetRTVCpuDescriptorHandle(),
			CRenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap)->GetDSVCpuDescriptorHandle()
		);

		//ビューポートを設定。
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(CRenderTarget::GetRenderTarget(enMainRT)->GetWidth());
		viewport.Height = static_cast<float>(CRenderTarget::GetRenderTarget(enMainRT)->GetHeight());
		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;
		rc.SetViewportAndScissor(viewport);

		//エフェクトを描画。
		EffectEngine::GetInstance()->Draw();

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
	}

	void CRenderingEngine::DrawWireFrame(CRenderContext& rc) {

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enMainRT));

		//ワイヤーフレームを描画。
		CPhysicsWorld::GetInstance()->DebubDrawWorld(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
	}
	void CRenderingEngine::CreateGBuffer(CRenderContext& rc) {

		//レンダリングターゲットを作成。
		CRenderTarget* rts[] = {
				CRenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap),   // 0番目のレンダリングターゲット
				CRenderTarget::GetGBufferRT(enNormalMap),  // 1番目のレンダリングターゲット
				CRenderTarget::GetGBufferRT(enWorldPosMap), // 2番目のレンダリングターゲット
				CRenderTarget::GetGBufferRT(enDepthMap), // 3番目のレンダリングターゲット
				CRenderTarget::GetGBufferRT(enocclusionAndSmoothAndMetaricMap) // 4番目のレンダリングターゲット
		};

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		//レンダーターゲットを設定。
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		//ビューポートを設定。
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		//アルベドマップと同じ幅と高さに設定する。
		viewport.Width = static_cast<float>(rts[0]->GetWidth());
		viewport.Height = static_cast<float>(rts[0]->GetHeight());
		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;
		rc.SetViewportAndScissor(viewport);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		//モデルの描画
		rc.SetRenderMode(CRenderContext::EnRender_Mode::enRenderMode_Normal);
		CGameObjectManager::GetInstance()->ExecuteRender(rc);

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void CRenderingEngine::ExecuteDeferredLighting(CRenderContext& rc) {

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(*CRenderTarget::GetRenderTarget(enMainRT));

		//ディファードライティング。
		m_deferredLightingSprite.Draw(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*CRenderTarget::GetRenderTarget(enMainRT));
	}

	void CRenderingEngine::SnapShotMainRenderTarget(CRenderContext& rc) {

		//レンダリングターゲットとして設定できるようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(m_snapShotMainRT);

		//レンダーターゲットとビューポートを設定。
		rc.SetRenderTargetAndViewport(m_snapShotMainRT);

		//メインレンダリングターゲットの絵をスナップショット。
		m_copyToMainRenderTargetSprite.Draw(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_snapShotMainRT);
	}

	void CRenderingEngine::CopyToFrameBuffer(CRenderContext& rc) {

		//メインレンダリングターゲットの絵をフレームバッファーにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		
		//最終スプライトを描画。
		m_copyToMainRenderTargetSprite.Draw(rc);
	}
}