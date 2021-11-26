#pragma once

namespace nsMyGame {

	//シャドウマップクラス
	class CShadowMap
	{
	public:
		/**
		 * @brief 初期化関数。
		*/
		void Init();

		/**
		 * @brief シャドウマップにブラーをかける関数。
		*/
		void ExecuteBlur(CRenderContext& rc);

		/**
		 * @brief シャドウマップのレンダリングターゲットを取得する関数。
		 * @return 
		*/
		CRenderTarget& GetRenderTarget() {

			return *m_shadowMap;
		}

		Texture& GetBokeShadowTexture() {

			return m_shadowBlur.GetBokeTexture();
		}
	private:
		CRenderTarget* m_shadowMap = nullptr;			//シャドウマップ
		nsPostEffect::CGaussianBlur m_shadowBlur;		//シャドウマップにかけるブラー
	};
}
