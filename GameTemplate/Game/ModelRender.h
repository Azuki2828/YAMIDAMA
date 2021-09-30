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
	 * @brief ワールド行列を更新する関数。(引数あり)
	 * @param pos 座標
	 * @param rot 回転
	 * @param sca 拡大率
	*/
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 sca)
	{
		m_model.UpdateWorldMatrix(pos, rot, sca);
	}

	/**
	 * @brief ワールド行列を更新する関数。(引数なし)
	*/
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

	/**
	 * @brief 回転率を設定する関数。
	 * @param rot 回転率
	*/
	void SetRotation(const Quaternion& rot) {

		m_rot = rot;
	}

	/**
	 * @brief 回転率を取得する関数。
	 * @return 回転率
	*/
	Quaternion GetRotation() {

		return m_rot;
	}

	/**
	 * @brief 拡大率を設定する関数。
	 * @param sca 拡大率
	*/
	void SetScale(const Vector3& sca) {

		m_sca = sca;
	}

	/**
	 * @brief 拡大率を取得する関数。
	 * @return 拡大率
	*/
	Vector3 GetScale() {

		return m_sca;
	}

	void SetModelUpAxis(EnModelUpAxis modelUpAxis) {
		m_modelInitData.m_modelUpAxis = modelUpAxis;
	}
private:
	const char* m_filePathTkm = nullptr;		//tkmファイルパス
	const char* m_filePathTks = nullptr;		//tksファイルパス


	Vector3 m_pos = Vector3::Zero;				//座標
	Quaternion m_rot = Quaternion::Identity;	//回転率
	Vector3 m_sca = Vector3::One;				//拡大率

	Model m_model;								//モデル
	ModelInitData m_modelInitData;				//モデルデータ
	Skeleton m_skeleton;						//スケルトン

	bool m_animFlg = false;						//アニメーション再生フラグ
	int m_animNum = 0;							//アニメーション数
	Animation m_animation;						//アニメーション
	AnimationClip* m_animationClip = nullptr;	//アニメーションクリップ
};

