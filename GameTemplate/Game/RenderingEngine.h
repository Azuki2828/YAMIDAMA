#pragma once
#include "PostEffect.h"

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
	 * @brief ポストエフェクトを初期化する関数。
	*/
	void InitPostEffect();

	/**
	 * @brief ディファードライティングで使用するスプライトを初期化する関数。
	*/
	void InitDeferredRenderingSprite();

	/**
	 * @brief フレームバッファにコピーするスプライトを作成する関数。
	*/
	void InitCopyToMainRenderTargetSprite();

	/**
	 * @brief シャドウマップを描画する関数。
	*/
	void DrawShadowMap();

	/**
	 * @brief フォントを描画する関数。
	*/
	void DrawFont();

	/**
	 * @brief ディファードレンダリングを実行する関数。
	*/
	void ExecuteDeferredRendering();

	/**
	 * @brief ディファードライティングを実行する関数。
	*/
	void ExecuteDeferredLighting();

	/**
	 * @brief 輝度テクスチャを描画する関数。
	*/
	void ExecuteDrawLuminanceTexture();

	/**
	 * @brief ガウシアンブラーをかける関数。
	*/
	void ExecuteGaussianBlur();

	/**
	 * @brief フレームバッファにスプライトをコピーする関数。
	*/
	void CopyToFrameBuffer();
private:
	static RenderingEngine* m_renderingEngine;	//レンダリングエンジンのインスタンス
	PostEffect* m_postEffect = nullptr;			//ポストエフェクト
	Sprite m_copyToMainRenderTargetSprite;		//メインレンダリングターゲットのスプライト
	Sprite m_deferredRenderingSprite;			//ディファードライティング用のスプライト
};

