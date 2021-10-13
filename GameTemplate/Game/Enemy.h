#pragma once
class CEnemy : public CIGameObject
{
private:
	/**
	 * @brief ステート。
	*/
	enum class enState {
		enState_Idle,
		enState_Attack,
		enState_Guard,
	};

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
	enState m_state = enState::enState_Idle;
	pybind11::module m_enemyPyModule;
};

