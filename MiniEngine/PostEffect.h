#pragma once

namespace nsMyGame {

	class PostEffect : public IGameObject
	{
	private:
		bool Start()override final;
	public:
		PostEffect() {

		}
		~PostEffect() {

		}
		void InitDeferrdSprite() {
			// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
			SpriteInitData spriteInitData;

			// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
			spriteInitData.m_width = c_deferredSpriteWH.x;
			spriteInitData.m_height = c_deferredSpriteWH.y;

			// ディファードライティングで使用するテクスチャを設定
			spriteInitData.m_textures[0] = &RenderTarget::GetGBufferRT(enAlbedoAndShadowReceiverFlgMap)->GetRenderTargetTexture();
			spriteInitData.m_textures[1] = &RenderTarget::GetGBufferRT(enNormalAndDepthMap)->GetRenderTargetTexture();
			spriteInitData.m_textures[2] = &RenderTarget::GetGBufferRT(enWorldPosMap)->GetRenderTargetTexture();

			spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
			spriteInitData.m_expandConstantBuffer = nsLight::LightManager::GetInstance()->GetLigData();
			spriteInitData.m_expandConstantBufferSize = sizeof(*nsLight::LightManager::GetInstance()->GetLigData());

			// 初期化データを使ってスプライトを作成
			m_defferdLightingSprite.Init(spriteInitData);
		}
		void InitLuminance(RenderTarget& renderTarget);
		Sprite& GetLuminanceSprite() { return m_sprite[0]; }
		Sprite& GetFinalSprite() { return m_sprite[1]; }
		Sprite& GetFXAASprite() { return m_fxaa.GetSprite(); }
		GaussianBlur& GetGaussianBlurSprite(int val_) { return m_gaussianBlur[val_]; }
		void InitGaussianBlur(RenderTarget& renderTarget);

		void InitFXAA();

		void RenderFXAA(RenderContext& rc);
	private:
		/*SpritePtr m_sprite[2];*/
		std::unique_ptr<Sprite[]> m_sprite = std::make_unique<Sprite[]>(2);
		std::unique_ptr<GaussianBlur[]> m_gaussianBlur = std::make_unique<GaussianBlur[]>(4);
		Sprite m_defferdLightingSprite;
		FXAA m_fxaa;
	};
}

