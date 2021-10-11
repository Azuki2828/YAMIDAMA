#pragma once
#include "Bloom.h"
#include "FXAA.h"

namespace nsMyGame {

	namespace nsPostEffect {

		//ポストエフェクトクラス

		class PostEffect
		{
		public:
			/**
			 * @brief 初期化関数。
			*/
			void Init();

			/**
			 * @brief 描画関数。
			 * @param rc レンダーコンテキスト
			*/
			void Render(RenderContext& rc);
		private:
			Bloom m_bloom;		//ブルーム
			FXAA m_fxaa;		//FXAA
		};
	}
}

