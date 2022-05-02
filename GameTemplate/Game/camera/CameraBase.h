#pragma once
namespace nsMyGame {

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
		 * @brief カメラを切り替えたときに呼び出される関数。
		*/
		virtual void Switched() = 0;

		/**
		 * @brief 各カメラの解放関数。
		*/
		virtual void Release(){}

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
	};
}

