#pragma once

namespace nsMyGame {

	//モデル管理クラス
	class CModelRender : public IGameObject
	{
		//コンスタントバッファに転送するデータ
		struct SModelData {
			int shadowReceiverFlg = 0;		//シャドウレシーバーフラグ
		};
	public:
		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

		/**
		 * @brief 描画関数。
		 * @param rc レンダーコンテキスト
		*/
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
		 * @param animNum アニメーション数。
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
		void UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& sca)
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
		const Vector3& GetPosition()const {

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
		const Quaternion& GetRotation()const {

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
		const Vector3& GetScale()const {

			return m_sca;
		}

		/**
		 * @brief シャドウキャスターフラグを設定する関数。
		 * @param flg シャドウキャスターフラグ。
		*/
		void SetShadowCasterFlag(bool flg) {

			m_shadowCasterFlag = flg;
		}

		/**
		 * @brief シャドウレシーバーフラグを設定する関数。
		 * @param flg シャドウレシーバーフラグ
		*/
		void SetShadowReceiverFlag(bool flg) {

			m_sModelData.shadowReceiverFlg = flg;
		}

		/**
		 * @brief モデルの上軸を設定する関数。
		 * @param modelUpAxis 上にしたい軸
		*/
		void SetModelUpAxis(EnModelUpAxis modelUpAxis) {
			m_modelInitData.m_modelUpAxis = modelUpAxis;
		}
	private:
		/**
		 * @brief シャドウモデルを作成する関数。
		*/
		void CreateShadowModel();

		/**
		 * @brief tkmファイルとtksファイルのパスを設定する関数。
		*/
		void SetFilePathTkmAndTks();
	private:
		const char* m_filePathTkm = nullptr;		//tkmファイルパス
		const char* m_filePathTks = nullptr;		//tksファイルパス

		bool m_shadowCasterFlag = false;			//シャドウキャスターフラグ。

		Vector3 m_pos = Vector3::Zero;				//座標
		Quaternion m_rot = Quaternion::Identity;	//回転率
		Vector3 m_sca = Vector3::One;				//拡大率

		Model m_model;								//モデル
		Model m_shadowModel;						//シャドウ作成用のモデル。
		SModelData m_sModelData;					//コンスタントバッファに転送するデータ
		ModelInitData m_modelInitData;				//モデルデータ
		Skeleton m_skeleton;						//スケルトン

		bool m_animFlg = false;						//アニメーション再生フラグ
		int m_animNum = 0;							//アニメーション数
		Animation m_animation;						//アニメーション
		AnimationClip* m_animationClip = nullptr;	//アニメーションクリップ
	};
}

