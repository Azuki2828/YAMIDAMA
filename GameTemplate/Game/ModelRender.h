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
	 * @brief アニメーションを設定する関数。
	 * @param animClip アニメーションクリップ
	 * @param animNum アニメーション番号
	*/
	void InitAnimation(AnimationClip* animClip, int animNum) {
		m_animationClip = animClip;
		m_animNum = animNum;
	}

	/**
	 * @brief アニメーションを再生するフラグ
	 * @param flg アニメーションフラグ
	*/
	void SetAnimFlg(bool flg) {
		m_animFlg = flg;
	}

	/**
	 * @brief アニメーションの再生。
	 * @param animNo アニメーションクリップの番号
	 * @param interpolateTime 補完時間
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}

	/**
	 * @brief ワールド行列を更新する関数。
	 * @param pos 座標
	 * @param rot 回転
	 * @param sca 拡大率
	*/
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 sca)
	{
		m_model.UpdateWorldMatrix(pos, rot, sca);
	}

	void UpdateWorldMatrix()
	{
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
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
	Skeleton m_skeleton;

	bool m_animFlg = false;
	int m_animNum = 0;
	Animation m_animation;
	AnimationClip* m_animationClip = nullptr;
};

