#pragma once

//モデル管理クラス

class ModelRender : public IGameObject
{
public:
	void Update()override final;
	void Render(RenderContext& rc)override final;
public:
	/**
	 * @brief 初期化関数。
	*/
	void Init();

	/**
	 * @brief tkmファイルを設定する関数。
	 * @param filePathTkm ファイル名(.tkm)
	*/
	void SetFilePathTkm(const char* filePathTkm) {
		m_filePathTkm = filePathTkm;
	}

	/**
	 * @brief tksファイルを設定する関数。
	 * @param filePathTks ファイル名(.tks)
	*/
	void SetFilePathTks(const char* filePathTks) {
		m_filePathTks = filePathTks;
	}

	/**
	 * @brief 座標を設定する関数。
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) {
		m_pos = pos;
	}

	/**
	 * @brief 座標を取得する関数。
	 * @return 座標
	*/
	Vector3 GetPosition() {

		return m_pos;
	}
private:
	const char* m_filePathTkm = nullptr;
	const char* m_filePathTks = nullptr;

	Vector3 m_pos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_sca = Vector3::One;

	Model m_model;
	ModelInitData m_modelInitData;
	Animation m_animation;
	AnimationClip* m_animationClip = nullptr;
	Skeleton m_skeleton;
};

