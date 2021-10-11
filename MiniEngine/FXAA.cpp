#include "stdafx.h"
#include "FXAA.h"

void FXAA::Init() {

    // 最終合成用のスプライトを初期化する
    SpriteInitData spriteInitData;
    spriteInitData.m_textures[0] = &RenderTarget::GetRenderTarget(enMainRT)->GetRenderTargetTexture();
    // 解像度はmainRenderTargetの幅と高さ
    spriteInitData.m_width = RenderTarget::GetRenderTarget(enMainRT)->GetWidth();
    spriteInitData.m_height = RenderTarget::GetRenderTarget(enMainRT)->GetHeight();
    // 2D用のシェーダーを使用する
    spriteInitData.m_fxFilePath = "Assets/shader/fxaa.fx";
    spriteInitData.m_vsEntryPointFunc = "VSMain";
    spriteInitData.m_psEntryPoinFunc = "PSMain";

    spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

    //解像度をGPUに送るための定数バッファを設定する。
    spriteInitData.m_expandConstantBuffer = (void*)&m_buffer;
    spriteInitData.m_expandConstantBufferSize = sizeof(SFXAABuffer) +
        (16 - (sizeof(SFXAABuffer) % 16));
    m_finalSprite.Init(spriteInitData);
}

void FXAA::Render(RenderContext& rc) {

    // レンダリングターゲットとして利用できるまで待つ
    rc.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enFXAART));
    // レンダリングターゲットを設定
    rc.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enFXAART));
    m_buffer.bufferW = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
    m_buffer.bufferH = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
    //描画。
    m_finalSprite.Draw(rc);
    // レンダリングターゲットへの書き込み終了待ち
    //メインレンダ―ターゲットをRENDERTARGETからPRESENTへ。
    rc.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enFXAART));
}