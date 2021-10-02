#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;


void RenderingEngine::Init() {

	CreateRenderTarget();

	InitPostEffect();

	InitCopyToMainRenderTargetSprite();
}

void RenderingEngine::Render() {

	DrawShadowMap();

	DrawModel();

	ExecuteDrawLuminanceTexture();

	ExecuteGaussianBlur();

	CopyToFrameBuffer();
}

void RenderingEngine::CreateRenderTarget() {

	RenderTarget::CreateMainRenderTarget();
	RenderTarget::CreateLuminanceRenderTarget();
	RenderTarget::CreateShadowMap();
}

void RenderingEngine::InitPostEffect() {

	m_postEffect = NewGO<PostEffect>(0);
	m_postEffect->InitLuminance(*RenderTarget::GetRenderTarget(enMainRT));
	m_postEffect->InitGaussianBlur(*RenderTarget::GetRenderTarget(enLuminanceRT));
}

void RenderingEngine::InitCopyToMainRenderTargetSprite() {

	SpriteInitData copyToMainRenderTargetSpriteInitData;
	copyToMainRenderTargetSpriteInitData.m_textures[0] = &RenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();
	copyToMainRenderTargetSpriteInitData.m_width = RENDER_TARGET_W1280H720.x;
	copyToMainRenderTargetSpriteInitData.m_height = RENDER_TARGET_W1280H720.y;
	copyToMainRenderTargetSpriteInitData.m_fxFilePath = SPRITE_SHADER_MONOCHROME_FILE_PATH;
	copyToMainRenderTargetSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_copyToMainRenderTargetSprite.Init(copyToMainRenderTargetSpriteInitData);
}

void RenderingEngine::DrawShadowMap() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	renderContext.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Shadow);

	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enShadowMap));

	renderContext.ClearRenderTargetView(*RenderTarget::GetRenderTarget(enShadowMap));

	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enShadowMap));
}

void RenderingEngine::DrawModel() {

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	renderContext.SetRenderMode(RenderContext::EnRender_Mode::enRenderMode_Normal);

	renderContext.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));

	renderContext.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));

	renderContext.ClearRenderTargetView(*RenderTarget::GetRenderTarget(enMainRT));

	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

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
	for (int i = 0; i < GAUSSIAN_BLUR_NUM; i++) {

		m_postEffect->GetGaussianBlurSprite(i).ExecuteOnGPU(renderContext, BLUR_POWER);
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