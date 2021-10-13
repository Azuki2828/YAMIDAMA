#pragma once
#include "stdafx.h"
#include "../../ExEngine/gameObject/IGameobject.h"

namespace nsMyGame {

	namespace nsLight {

		class CLightBase : public CIGameObject
		{
			bool Start();
		public:
			~CLightBase();
			virtual bool StartSub() = 0;
			//ライトのデータを取得する関数。
			virtual void* GetLigData() = 0;
		};
	}
}
