#include "stdafx.h"
#include "PostEffectComponentBase.h"

namespace nsMyGame {

	namespace nsPostEffect {

		void PostEffectComponentBase::Init() {

			//派生クラスの初期化。
			OnInit();


			//派生クラスでコピー許可されているなら、コピー用のスプライトを作成する。
			//ブルーム等、メインレンダリングターゲットに直接加算合成するものは派生クラスのfalseを使用する。
			if (IsCopyResultTextureToMainRenderTarget()) {
				SpriteInitData initData;
				initData.m_width = RenderTarget::GetRenderTarget(enMainRT)->GetWidth();
				initData.m_height = RenderTarget::GetRenderTarget(enMainRT)->GetHeight();
				initData.m_colorBufferFormat = RenderTarget::GetRenderTarget(enMainRT)->GetColorBufferFormat();
				initData.m_fxFilePath = c_fxFilePathCopySprite;
				initData.m_textures[0] = &GetResultTexture();
				m_copyMainRtSprite.Init(initData);
			}
		}

		void PostEffectComponentBase::Render(RenderContext& rc) {

			//派生クラスの描画関数呼び出し。
			OnRender(rc);

			//auto& mainRT = *RenderTarget::GetRenderTarget(enMainRT);



			//派生クラスでコピー許可されているなら、ポストエフェクトの結果をメインレンダリングターゲットに反映。
			if (IsCopyResultTextureToMainRenderTarget()) {
				// レンダリングターゲットとして利用できるまで待つ
				rc.WaitUntilToPossibleSetRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
				// レンダリングターゲットを設定
				rc.SetRenderTargetAndViewport(*RenderTarget::GetRenderTarget(enMainRT));
				// ポストエフェクトの結果をメインレンダリングターゲットに反映。
				m_copyMainRtSprite.Draw(rc);
				// レンダリングターゲットへの書き込み終了待ち
				rc.WaitUntilFinishDrawingToRenderTarget(*RenderTarget::GetRenderTarget(enMainRT));
			}
		}
	}
}