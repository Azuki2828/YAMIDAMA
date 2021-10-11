#include "stdafx.h"
#include "PostEffect.h"


namespace nsMyGame {

	namespace nsPostEffect {

		void PostEffect::Init() {

			//ブルームの初期化。
			m_bloom.Init();

			//FXAAの初期化。
			m_fxaa.Init();
		}

		void PostEffect::Render(RenderContext& rc) {

			//ブルームの描画。
			m_bloom.Render(rc);

			//FXAAの描画。
			m_fxaa.Render(rc);
		}
	}
}