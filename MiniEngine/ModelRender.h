#pragma once

namespace nsMyGame {

	//モデル管理クラス
	class CModelRender : public CIGameObject
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
		void Render(CRenderContext& rc)override final;
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
		void InitAnimation(CAnimationClip* animClip, int animNum) {
			m_animationClip = animClip;
			m_animNum = animNum;
			SetAnimFlg(true);
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
		void UpdateWorldMatrix(const CVector3& pos, const CQuaternion& rot, const CVector3& sca)
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
		void SetPosition(const CVector3& pos) {
			m_pos = pos;
		}

		/**
		 * @brief 座標を取得する関数。
		 * @return 座標
		*/
		const CVector3& GetPosition()const {

			return m_pos;
		}

		/**
		 * @brief 回転率を設定する関数。
		 * @param rot 回転率
		*/
		void SetRotation(const CQuaternion& rot) {

			m_rot = rot;
		}

		/**
		 * @brief 回転率を取得する関数。
		 * @return 回転率
		*/
		const CQuaternion& GetRotation()const {

			return m_rot;
		}

		/**
		 * @brief 拡大率を設定する関数。
		 * @param sca 拡大率
		*/
		void SetScale(const CVector3& sca) {

			m_sca = sca;
		}

		/**
		 * @brief 拡大率を取得する関数。
		 * @return 拡大率
		*/
		const CVector3& GetScale()const {

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

		/**
		 * @brief モデルを取得する関数。
		 * @return モデル
		*/
		CModel* GetModel() { return &m_model; }

		Skeleton* GetSkeleton() {

			return &m_skeleton;
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

		CVector3 m_pos = CVector3::Zero;				//座標
		CQuaternion m_rot = CQuaternion::Identity;	//回転率
		CVector3 m_sca = CVector3::One;				//拡大率

		CModel m_model;								//モデル
		CModel m_shadowModel;						//シャドウ作成用のモデル。
		SModelData m_sModelData;					//コンスタントバッファに転送するデータ
		ModelInitData m_modelInitData;				//モデルデータ
		Skeleton m_skeleton;						//スケルトン

		bool m_animFlg = false;						//アニメーション再生フラグ
		int m_animNum = 0;							//アニメーション数
		Animation m_animation;						//アニメーション
		CAnimationClip* m_animationClip = nullptr;	//アニメーションクリップ
	};
}

