#pragma once
#include "../MiniEngine/postEffect/PostEffect.h"

namespace nsMyGame {
	//レンダリングエンジンクラス
	class RenderingEngine
	{
	public:
		/**
		 * @brief レンダリングエンジンを作成する関数。
		*/
		static void CreateRenderingEngine() {

			m_renderingEngine = new RenderingEngine;
		}

		/**
		 * @brief レンダリングエンジンを取得する関数。
		 * @return レンダリングエンジン
		*/
		static RenderingEngine* GetInstance() {

			return m_renderingEngine;
		}

		/**
		 * @brief 初期化関数。
		*/
		void Init();

		/**
		 * @brief 描画関数。
		*/
		void Render();
	private:
		/**
		 * @brief レンダリングターゲットを作成する関数。
		*/
		void CreateRenderTarget();

		/**
		 * @brief ディファードライティングで使用するスプライトを初期化する関数。
		*/
		void InitDeferredRenderingSprite();

		/**
		 * @brief メインレンダリングターゲットのコピーを取るためのレンダーターゲットを作成する関数。
		*/
		void CreateSnapShotMainRT();
		/**
		 * @brief フレームバッファにコピーするスプライトを作成する関数。
		*/
		void InitCopyToMainRenderTargetSprite();

		/**
		 * @brief シャドウマップを描画する関数。
		 * @param rc レンダーコンテキスト。
		*/
		void DrawShadowMap(RenderContext& rc);

		/**
		 * @brief フォントを描画する関数。
		 * @param rc レンダーコンテキスト。
		*/
		void DrawFont(RenderContext& rc);

		/**
		 * @brief ディファードレンダリングを実行する関数。
		 * @param rc レンダーコンテキスト。
		*/
		void ExecuteDeferredRendering(RenderContext& rc);

		/**
		 * @brief ディファードライティングを実行する関数。
		 * @param rc レンダーコンテキスト。
		*/
		void ExecuteDeferredLighting(RenderContext& rc);

		/**
		 * @brief フレームバッファ用に、メインレンダリングターゲットの内容をコピーする関数。
		 * @param rc レンダーコンテキスト。
		*/
		void SnapShotMainRenderTarget(RenderContext& rc);


		/**
		 * @brief フレームバッファにスプライトをコピーする関数。
		 * @param rc レンダーコンテキスト。
		*/
		void CopyToFrameBuffer(RenderContext& rc);

	private:
		static RenderingEngine* m_renderingEngine;	//レンダリングエンジンのインスタンス
		RenderTarget m_snapShotMainRT;				//メインレンダリングターゲットのスナップショットを取るためのレンダリングターゲット
		nsPostEffect::PostEffect m_postEffect;		//ポストエフェクト
		Sprite m_copyToMainRenderTargetSprite;		//メインレンダリングターゲットのスプライト
		Sprite m_deferredRenderingSprite;			//ディファードライティング用のスプライト
	};
}

