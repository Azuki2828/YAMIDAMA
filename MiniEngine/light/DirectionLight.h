#pragma once
#include "constLight.h"
#include "LightBase.h"

namespace nsMyGame {

	namespace nsLight {

		//ディレクションライトのデータ
		struct DirectionLigData {
			CVector3 Dir = CVector3::Zero;	//方向
			float pad = 0.0f;				//パディング
			CVector4 Col = CVector4::White;	//色
		};

		//平行光源クラス
		class CDirectionLight : public CLightBase
		{
			/**
			 * @brief オーバーライドされた開始処理関数。
			 * @return 成功した？
			*/
			bool StartSub();
		public:
			/**
			 * @brief ライトの方向を設定する関数。
			 * @param _x x方向
			 * @param _y y方向
			 * @param _z z方向
			*/
			void SetLigDirection(float _x = 0.0f, float _y = -1.0f, float _z = 1.0f) {
				m_dirLigData.Dir.x = _x;
				m_dirLigData.Dir.y = _y;
				m_dirLigData.Dir.z = _z;
				//正規化。
				m_dirLigData.Dir.Normalize();
			}

			/**
			 * @brief ライトの方向を設定する関数。
			 * @param dir 方向
			*/
			void SetLigDirection(const CVector3& dir) {
				m_dirLigData.Dir = dir;
				//正規化。
				m_dirLigData.Dir.Normalize();
			}

			/**
			 * @brief ディレクションライトを取得。
			 * @return ディレクションライト
			*/
			const CVector3* GetLigDirection() {
				return &m_dirLigData.Dir;
			}

			/**
			 * @brief ライトの色を設定。
			 * @param _x 赤成分
			 * @param _y 緑成分
			 * @param _z 青成分
			*/
			void SetLigColor(float _x = 50.0f, float _y = 50.0f, float _z = 50.0f) {
				m_dirLigData.Col.x = _x;
				m_dirLigData.Col.y = _y;
				m_dirLigData.Col.z = _z;
			}

			/**
			 * @brief ライトの色を設定。
			 * @param col 色情報(RGB)
			*/
			void SetLigColor(const CVector3& col) {
				m_dirLigData.Col = col;
			}

			/**
			 * @brief ライトのデータ取得。
			 * @return ライトのデータ
			*/
			void* GetLigData() { return &m_dirLigData; }
		private:
			DirectionLigData m_dirLigData;					//ディレクションライトのデータ
		};
	}
}