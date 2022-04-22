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

			//切り替わって１フレームたった。
			if (m_switched) { m_switched = false; }
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
		 * @brief カメラを切り替えたことを知らせる関数。
		*/
		void Switched() {

			m_switched = true;
		}

		/**
		 * @brief ロックオン中の敵の座標を取得する関数。
		 * @return ロックオン中の敵の座標
		*/
		virtual const CVector3& GetLockOnEnemyPosition()const {

			MessageBoxA(nullptr, "正しい情報が呼び出されていません。", "エラー", MB_OK);
			return CVector3::Zero;
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
		bool m_switched = false;	//カメラが切り替わった？
	};
}

