#pragma once
#include "Sprite.h"

namespace nsMyGame {

	namespace nsPostEffect {
		
		
		//ガウシアンブラークラス
		
		class CGaussianBlur {
		public:
			/**
			 * @brief 初期化関数。
			 * @param originalTexture ガウシアンブラーをかけるオリジナルテクスチャ。
			*/
			void Init(Texture* originalTexture);

			/**
			 * @brief ガウシアンブラーをGPU上で実行する関数。
			 * @param rc レンダーコンテキスト
			 * @param blurPower ブラーの強さ。値が大きいほどボケが強くなる。
			 * 
			 * 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
			*/
			void ExecuteOnGPU(RenderContext& rc, float blurPower);

			/**
			 * @brief ボケテクスチャを取得する関数。
			 * @return ボケテクスチャ
			*/
			Texture& GetBokeTexture()
			{
				return m_yBlurRenderTarget.GetRenderTargetTexture();
			}
		private:

			/**
			 * @brief レンダリングターゲットを初期化する関数。
			*/
			void InitRenderTargets();

			/**
			 * @brief スプライトを初期化する関数。
			*/
			void InitSprites();

			/**
			 * @brief 重みテーブルを更新する関数。
			 * @param blurPower 重み
			*/
			void UpdateWeightsTable(float blurPower);

			/**
			 * @brief 縦ブラーを実行。
			*/
			void ExecuteVerticalBlur(RenderContext& rc);

			/**
			 * @brief 横ブラーを実行。
			*/
			void ExecuteBesideBlur(RenderContext& rc);

		private:
			float m_weights[c_gaussianBlurNumWeight];			//重みテーブル
			Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ
			RenderTarget m_xBlurRenderTarget;		//横ボケ画像を描画するレンダリングターゲット
			RenderTarget m_yBlurRenderTarget;		//縦ボケ画像を描画するレンダリングターゲット
			Sprite m_xBlurSprite;					//横ボケ画像を描画するためのスプライト
			Sprite m_yBlurSprite;					//縦ボケ画像を描画するためのスプライト
		};
	}
}
