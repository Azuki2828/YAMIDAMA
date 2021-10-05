#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;


void RenderingEngine::Init() {

	//レンダリングターゲットを作成。
	CreateRenderTarget();

	//ポストエフェクトを初期化。
	InitPostEffect();

	//ディファードライティング用のスプライトを初期化。
	InitDeferredRenderingSprite();

	//メインレンダリングターゲットのスプライトを初期化。
	InitCopyToMainRenderTargetSprite();
}

void RenderingEngine::Render() {

	//シャドウマップを描画。
	DrawShadowMap();

	//ディファードレンダリング
	ExecuteDeferredRendering();

	//ディファードライティング。
	ExecuteDeferredLighting();

	//輝度テクスチャを作成。
	ExecuteDrawLuminanceTexture();

	//ブルーム。
	ExecuteGaussianBlur();

	DrawFont();
	//フレームバッファを描画。
	CopyToFrameBuffer();
}

void RenderingEngine::CreateRenderTarget() {

	//レンダリングターゲットを作成。
	RenderTarget::CreateMainRenderTarget();
	RenderTarget::CreateLuminanceRenderTarget();
	RenderTarget::CreateShadowMap();
	RenderTarget::CreateAlbedoAndShadowReceiverFlagRenderTarget();
	RenderTarget::CreateNormalAndDepthRenderTarget();
	RenderTarget::CreateWorldPosRenderTarget();
}

void RenderingEngine::InitPostEffect() {

	//ブルームに必要なテクスチャを初期化。
	m_postEffect = NewGO<PostEffect>(0);
	m_postEffect->InitLuminance(*RenderTarget::GetRenderTarget(enMainRT));
	m_postEffect->InitGaussianBlur(*RenderTarget::GetRenderTarget(enLuminanceRT));
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
	spriteInitData.m_expandConstantBuffer = LightManager::GetInstance()->GetLigData();
	spriteInitData.m_expandConstantBufferSize = sizeof(*LightManager::GetInstance()->GetLigData());
	spriteInitData.m_expandShaderResoruceView = &RenderTarget::GetRenderTarget(enShadowMap)->GetRenderTargetTexture();

	// 初期化オブジェクトを使って、スプライトを初期化する
	m_deferredRenderingSprite.Init(spriteInitData);
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

void RenderingEngine::DrawShadowMap() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();
	
	//描画モードをシャドウマップ用に設定する。
	renderContext.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Shadow);

	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enShadowMap));

	renderContext.ClearRenderTargetView(*RenderTarget::GetRenderTarget(enShadowMap));

	//シャドウモデルを描画。
	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));
}

void RenderingEngine::DrawFont() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	renderContext.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Font);

	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
}

void RenderingEngine::ExecuteDeferredRendering() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	RenderTarget* rts[] = {
			RenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap),   // 0番目のレンダリングターゲット
			RenderTarget::GetGBufferRT(enNormalAndDepthMap),  // 1番目のレンダリングターゲット
			RenderTarget::GetGBufferRT(enWorldPosMap) // 2番目のレンダリングターゲット
	};

	// まず、レンダリングターゲットとして設定できるようになるまで待つ
	renderContext.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

	// レンダリングターゲットを設定
	renderContext.SetRenderTargets(ARRAYSIZE(rts), rts);

	//ビューポートを設定。
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	//アルベドマップと同じ幅と高さに設定する。
	viewport.Width = static_cast<float>(rts[0]->GetWidth());
	viewport.Height = static_cast<float>(rts[0]->GetHeight());
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;
	renderContext.SetViewportAndScissor(viewport);

	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

	//モデルの描画
	renderContext.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Normal);
	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	// レンダリングターゲットへの書き込み待ち
	renderContext.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
}

void RenderingEngine::ExecuteDeferredLighting() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

	m_deferredRenderingSprite.Draw(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
}

void RenderingEngine::ExecuteDrawLuminanceTexture() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	auto& luminanceRenderTarget = *RenderTarget::GetRenderTarget(enLuminanceRT);

	// 輝度抽出用のレンダリングターゲットに変更
	renderContext.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);

	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(luminanceRenderTarget);

	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(luminanceRenderTarget);

	// 輝度抽出を行う
	m_postEffect->GetLuminanceSprite().Draw(renderContext);

	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);
}

void RenderingEngine::ExecuteGaussianBlur() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//ガウシアンブラーを4回実行する
	for (int i = 0; i < c_gaussianBlurNum; i++) {

		m_postEffect->GetGaussianBlurSprite(i).ExecuteOnGPU(renderContext, c_blurPower);
	}

	//4枚のボケ画像を合成してメインレンダリングターゲットに加算合成
	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

	m_postEffect->GetFinalSprite().Draw(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
}

void RenderingEngine::CopyToFrameBuffer() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// メインレンダリングターゲットの絵をフレームバッファーにコピー
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	m_copyToMainRenderTargetSprite.Draw(renderContext);
}