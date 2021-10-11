#include "stdafx.h"
#include "RenderingEngine.h"

namespace nsMyGame {

	//レンダリングエンジンのインスタンス
	RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;


	void RenderingEngine::Init() {

		//レンダリングターゲットを作成。
		CreateRenderTarget();

		//ポストエフェクトを初期化。
		m_postEffect.Init();

		//ディファードライティング用のスプライトを初期化。
		InitDeferredRenderingSprite();

		//メインレンダリングターゲットのスプライトを初期化。
		InitCopyToMainRenderTargetSprite();
	}

	void RenderingEngine::Render() {

		//レンダーコンテキストを取得。
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		//シャドウマップを描画。
		DrawShadowMap(renderContext);

		//ディファードレンダリング。
		ExecuteDeferredRendering(renderContext);

		//ディファードライティング。
		ExecuteDeferredLighting(renderContext);

		//メインレンダリングターゲットの絵をスナップショット。
		SnapShotMainRenderTarget(renderContext);

		//ポストエフェクト。
		m_postEffect.Render(renderContext);

		//フォントを描画。
		DrawFont(renderContext);

		//フレームバッファを描画。
		CopyToFrameBuffer(renderContext);
	}

	void RenderingEngine::CreateRenderTarget() {

		//レンダリングターゲットを作成。
		RenderTarget::CreateMainRenderTarget();
		RenderTarget::CreateShadowMap();
		RenderTarget::CreateAlbedoAndShadowReceiverFlagRenderTarget();
		RenderTarget::CreateNormalAndDepthRenderTarget();
		RenderTarget::CreateWorldPosRenderTarget();
		CreateSnapShotMainRT();
	}

	void RenderingEngine::InitDeferredRenderingSprite() {

		//ディファードレンダリングに必要なデータを設定する。
		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &RenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[1] = &RenderTarget::GetGBufferRT(enNormalAndDepthMap)->GetRenderTargetTexture();
		spriteInitData.m_textures[2] = &RenderTarget::GetGBufferRT(enWorldPosMap)->GetRenderTargetTexture();
		spriteInitData.m_width = c_renderTargetW1280H720.x;
		spriteInitData.m_height = c_renderTargetW1280H720.y;
		spriteInitData.m_fxFilePath = c_fxFilePath_DeferredLighting;
		spriteInitData.m_expandConstantBuffer = nsLight::LightManager::GetInstance()->GetLigData();
		spriteInitData.m_expandConstantBufferSize = sizeof(*nsLight::LightManager::GetInstance()->GetLigData());
		spriteInitData.m_expandShaderResoruceView = &RenderTarget::GetRenderTarget(enShadowMap)->GetRenderTargetTexture();

		// 初期化オブジェクトを使って、スプライトを初期化する
		m_deferredRenderingSprite.Init(spriteInitData);
	}


	void RenderingEngine::CreateSnapShotMainRT() {

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
	void RenderingEngine::InitCopyToMainRenderTargetSprite() {

		SpriteInitData copyToMainRenderTargetSpriteInitData;

		//テクスチャにはメインレンダリングターゲットのテクスチャカラーを指定。
		copyToMainRenderTargetSpriteInitData.m_textures[0] = &RenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();
		copyToMainRenderTargetSpriteInitData.m_width = c_renderTargetW1280H720.x;
		copyToMainRenderTargetSpriteInitData.m_height = c_renderTargetW1280H720.y;
		copyToMainRenderTargetSpriteInitData.m_fxFilePath = c_fxFilePath_Sprite;
		copyToMainRenderTargetSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_copyToMainRenderTargetSprite.Init(copyToMainRenderTargetSpriteInitData);
	}

	void RenderingEngine::DrawShadowMap(RenderContext& rc) {

		//描画モードをシャドウマップ用に設定する。
		rc.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Shadow);

		//レンダリングターゲットを設定。
		rc.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));

		//ビューポートを設定。
		rc.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enShadowMap));

		//レンダーターゲットをクリア。
		rc.ClearRenderTargetView(*RenderTarget::GetRenderTarget(enShadowMap));

		//シャドウモデルを描画。
		GameObjectManager::GetInstance()->ExecuteRender(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));
	}

	void RenderingEngine::DrawFont(RenderContext& rc) {

		//レンダーモードをフォント用にする。
		rc.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Font);

		//レンダリングターゲットを設定。
		rc.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

		//ビューポートを設定。
		rc.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

		//シャドウモデルを描画。
		GameObjectManager::GetInstance()->ExecuteRender(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
	}

	void RenderingEngine::ExecuteDeferredRendering(RenderContext& rc) {

		//レンダリングターゲットを作成。
		RenderTarget* rts[] = {
				RenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap),   // 0番目のレンダリングターゲット
				RenderTarget::GetGBufferRT(enNormalAndDepthMap),  // 1番目のレンダリングターゲット
				RenderTarget::GetGBufferRT(enWorldPosMap) // 2番目のレンダリングターゲット
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
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
		rc.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Normal);
		GameObjectManager::GetInstance()->ExecuteRender(rc);

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void RenderingEngine::ExecuteDeferredLighting(RenderContext& rc) {

		//レンダリングターゲットを設定。
		rc.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

		//ビューポートを設定。
		rc.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

		//ディファードライティング。
		m_deferredRenderingSprite.Draw(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
	}

	void RenderingEngine::SnapShotMainRenderTarget(RenderContext& rc) {

		//レンダリングターゲットを設定。
		rc.WaitUntilToPossibleSetRenderTarget(m_snapShotMainRT);

		//ビューポートを設定。
		rc.SetRenderTargetAndViewport(m_snapShotMainRT);

		//メインレンダリングターゲットの絵をスナップショット。
		m_copyToMainRenderTargetSprite.Draw(rc);

		//描き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_snapShotMainRT);
	}

	void RenderingEngine::CopyToFrameBuffer(RenderContext& rc) {

		//メインレンダリングターゲットの絵をフレームバッファーにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		
		//最終スプライトを描画。
		m_copyToMainRenderTargetSprite.Draw(rc);
	}
}