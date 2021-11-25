#include "stdafx.h"
#include "EffectManager.h"

namespace nsMyGame {

	namespace nsEffect {

		CEffectManager* CEffectManager::m_effectManager = nullptr;	//エフェクト管理クラスのインスタンス

		void CEffectManager::Init(const char16_t* filePath) {

			Effect* effect = NewGO<Effect>(enPriority_Zeroth);
			effect->Init(filePath);
		}
	}
}