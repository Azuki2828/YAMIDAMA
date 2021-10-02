#pragma once
#include "PostEffect.h"

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

	void Render();
private:
	void CreateRenderTarget();

	void InitPostEffect();

	void InitCopyToMainRenderTargetSprite();

	void DrawShadowMap();

	void DrawModel();

	void ExecuteDrawLuminanceTexture();

	void ExecuteGaussianBlur();

	void CopyToFrameBuffer();
private:
	static RenderingEngine* m_renderingEngine;	//レンダリングエンジンのインスタンス
	PostEffect* m_postEffect = nullptr;
	Sprite m_copyToMainRenderTargetSprite;
};

