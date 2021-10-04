#pragma once

enum EnAnimationList {
	enAnim_Walk,
	enAnim_Idle,
	enAnim_Death,

	enAnimNum

};
class Player : public IGameObject
{
	enum class EnPlayerState {
		enState_Normal,
		enState_Yoi,
		enState_Death,

		enStateNum
	};
	/**
	 * @brief モジュールをインポートする関数。
	 * @param moduleName モジュール名(Pythonファイル名)
	*/
	void ImportModule(const char* moduleName) {
		// モジュールをインポート
		try {
			m_playerPyModule = pybind11::module::import(moduleName);
		}
		catch (pybind11::error_already_set& e) {
			MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
		}
	}

	/**
	 * @brief ライトカメラを更新する関数。
	*/
	void LightCameraUpdate();
public:
	bool Start()override final;
	void Update()override final;

	const Vector3& GetPosition()const {

		return m_pos;
	}

	NMyRenderer::ModelRender* GetModelRender() {

		return m_modelRender;
	}


	void SetYoiParam(const int param) {

		m_yoiParam = param;
	}
	int GetYoiParam() {

		return m_yoiParam;
	}

	void Move();

	void FontUpdate();

	void SetState(int state) {
		switch (state) {
		case 0:
			m_playerState = EnPlayerState::enState_Normal;
			break;
		case 1:
			m_playerState = EnPlayerState::enState_Yoi;
			break;
		case 2:
			m_playerState = EnPlayerState::enState_Death;
			break;
		}
		
	}
private:
	int m_yoiParam = 0;
	EnPlayerState m_playerState = EnPlayerState::enState_Normal;
	NMyRenderer::ModelRender* m_modelRender = nullptr;
	AnimationClip m_animationClip[enAnimNum];

	Vector3 m_pos = {0.0f,0.0f,0.0f};
	pybind11::module m_playerPyModule;

	FontRender* m_fontRender = nullptr;
};

