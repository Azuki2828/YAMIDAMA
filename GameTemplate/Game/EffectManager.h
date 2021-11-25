#pragma once

namespace nsMyGame {

	namespace nsEffect {

		//エフェクト管理クラス
		class CEffectManager
		{
		private:
			//エフェクトのデータ
			struct SEffectData {

				int effectNum = -1;								//エフェクト番号
				CVector3 position = CVector3::Zero;				//座標
				CQuaternion rotation = CQuaternion::Identity;	//回転
				CVector3 scale = CVector3::One;					//拡大
			};
		public:
			/**
			 * @brief エフェクト管理クラスのインスタンスを作成する関数。
			*/
			static void CreateInstance() {

				m_effectManager = new CEffectManager;
			}

			/**
			 * @brief エフェクト管理クラスのインスタンスを取得する関数。
			 * @return エフェクト管理クラスのインスタンス
			*/
			static CEffectManager* GetInstance() {

				return m_effectManager;
			}

			/**
			 * @brief 初期化関数。
			 * @param filePath ファイルパス
			*/
			void Init(const char16_t* filePath);

			void Play(const int efkNum) {


			}
			/**
			 * @brief 座標を設定する関数。
			 * @param efkNum エフェクト番号
			 * @param pos 座標
			*/
			void SetPosition(const int efkNum, const CVector3& pos) {

				m_effectList[efkNum]->position = pos;
			}

			/**
			 * @brief 座標を取得する関数。
			 * @param efkNum エフェクト番号
			 * @return 座標
			*/
			const CVector3& GetPosition(const int efkNum) {

				return m_effectList[efkNum]->position;
			}

			/**
			 * @brief 回転を設定する関数。
			 * @param efkNum エフェクト番号
			 * @param rot 回転
			*/
			void SetRotation(const int efkNum, const CQuaternion& rot) {

				m_effectList[efkNum]->rotation = rot;
			}

			/**
			 * @brief 回転を取得する関数。
			 * @param efkNum エフェクト番号
			 * @return 回転
			*/
			const CQuaternion& GetRotation(const int efkNum) {

				return m_effectList[efkNum]->rotation;
			}

			/**
			 * @brief 拡大を設定する関数。
			 * @param efkNum エフェクト番号
			 * @param sca 拡大
			*/
			void SetScale(const int efkNum, const CVector3& sca) {

				m_effectList[efkNum]->scale = sca;
			}

			/**
			 * @brief 拡大を取得する関数。
			 * @param efkNum エフェクト番号
			 * @return 拡大
			*/
			const CVector3& GetScale(const int efkNum) {

				return m_effectList[efkNum]->scale;
			}
		private:
			int m_effectNum = 0;						//エフェクトの数
			static CEffectManager* m_effectManager;		//エフェクト管理クラスのインスタンス
			std::vector<SEffectData*> m_effectList;		//エフェクトのリスト
		};
	}
}

