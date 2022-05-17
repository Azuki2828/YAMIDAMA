#pragma once
namespace nsMyGame {

	namespace nsCamera {

		//カメラクラス
		class CCameraBase
		{
		public:
			/**
			 * @brief 初期化関数。
			 * @return 成功した？
			*/
			bool Start() {

				return StartSub();
			}

			/**
			 * @brief 更新関数。
			*/
			void Update() {

				UpdateSub();
			}

			/**
			 * @brief 視点の座標を取得する関数。
			 * @return 視点の座標
			*/
			const CVector3& GetPosition()const {

				return m_position;
			}

			/**
			 * @brief 注視点の座標を取得する関数。
			 * @return 注視点の座標
			*/
			const CVector3& GetTarget()const {

				return m_target;
			}

			/**
			 * @brief カメラが有効か調べる関数。
			 * @return カメラが有効？
			*/
			const bool IsEnable()const {

				return m_isEnable;
			}

			/**
			 * @brief カメラを切り替えたときに呼び出される関数。
			*/
			void Switched() {

				//カメラを有効化。
				m_isEnable = true;

				//サブクラスでのカメラ切り替わった時の処理を呼び出す。
				SubSwitched();
			}

			/**
			 * @brief サブクラスでのカメラ切り替わった時用の関数。
			*/
			virtual void SubSwitched() {}

			/**
			 * @brief 解放関数。
			*/
			void Release() {

				//カメラを無効化。
				m_isEnable = false;

				//サブクラスの解放。
				SubRelease();
			}

			/**
			 * @brief 各カメラの解放関数。
			*/
			virtual void SubRelease() {}

			/**
			 * @brief カメラの切り替えが可能か？
			 * @return 成功した？
			*/
			virtual const bool CanSwitch() {

				return true;
			}
		protected:
			/**
			 * @brief 派生クラスのStart()関数。
			 * @return 成功した？
			*/
			virtual bool StartSub() = 0;

			/**
			 * @brief 派生クラスのUpdate()関数。
			*/
			virtual void UpdateSub() = 0;
		protected:
			CVector3 m_position;		//視点
			CVector3 m_target;			//注視点
			static CVector3 m_cameraToPlayerVec;	//カメラからプレイヤーに伸びるベクトル
			bool m_isEnable = false;	//有効かどうか
		};
	}
}

