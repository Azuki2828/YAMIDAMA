#include "stdafx.h"
#include "PostEffect.h"


namespace nsMyGame {

	bool PostEffect::Start() {

		return true;
	}
	void PostEffect::InitLuminance(RenderTarget& renderTarget) {
		// 輝度抽出用のスプライトを初期化
		// 初期化情報を作成する
		SpriteInitData luminanceSpriteInitData;

		// 輝度抽出用のシェーダーのファイルパスを指定する
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";

		// 頂点シェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

		// ピクセルシェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

		// スプライトの幅と高さはluminnceRenderTargetと同じ
		luminanceSpriteInitData.m_width = 1280;
		luminanceSpriteInitData.m_height = 720;

		// テクスチャはメインレンダリングターゲットのカラーバッファー
		luminanceSpriteInitData.m_textures[0] = &renderTarget.GetRenderTargetTexture();

		// 描き込むレンダリングターゲットのフォーマットを指定する
		luminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		// 作成した初期化情報をもとにスプライトを初期化する
		m_sprite[0].Init(luminanceSpriteInitData);
	}

	void PostEffect::InitGaussianBlur(RenderTarget& renderTarget) {

		//
		m_gaussianBlur[0].Init(&renderTarget.GetRenderTargetTexture());
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

		// step-2 ボケ画像を合成して書き込むためのスプライトを初期化

		SpriteInitData finalSpriteInitData;

		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

		finalSpriteInitData.m_width = 1280;
		finalSpriteInitData.m_height = 720;

		finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		finalSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_sprite[1].Init(finalSpriteInitData);
	}

	void PostEffect::InitFXAA() {
		m_fxaa.Init();
	}

	void PostEffect::RenderFXAA(RenderContext& rc) {

		m_fxaa.Render(rc);
	}
}