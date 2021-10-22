#pragma once
#include "EnemyAction.h"

namespace nsMyGame {

	namespace nsEnemy {

		class CEnemy : public CIGameObject
		{
		private:

			/**
			 * @brief モジュールをインポートする関数。
			 * @param moduleName モジュール名(Pythonファイル名)
			*/
			void ImportModule(const char* moduleName) {
				// モジュールをインポート
				try {
					m_enemyPyModule = pybind11::module::import(moduleName);
				}
				catch (pybind11::error_already_set& e) {
					MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
				}
			}
		public:
			bool Start() override;
			void Update() override;
			void CallTest() {
				MessageBox(nullptr, L"Success Connect to Class Function!!", L"通知", MB_OK);
			}
			void Render(CRenderContext& rc);
		private:
			EnEnemyState m_enemyState = enState_Idle;
			pybind11::module m_enemyPyModule;
		};
	}
}

