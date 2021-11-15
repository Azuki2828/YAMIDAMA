#pragma once

namespace nsMyGame {

	//ドアクラス
	class CDoor : public CIGameObject
	{
	public:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start() override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

		/**
		 * @brief 座標を設定する関数。
		 * @param pos 座標
		*/
		void SetPosition(const CVector3& pos) {

			m_position = pos;
		}

		/**
		 * @brief 回転を設定する関数。
		 * @param rot 回転
		*/
		void SetRotation(const CQuaternion& rot) {

			m_rotation = rot;
		}

		/**
		 * @brief 拡大を設定する関数。
		 * @param scale 拡大
		*/
		void SetScale(const CVector3& scale) {

			m_scale = scale;
		}

	private:
		/**
		 * @brief 鍵がかかっている？
		 * @return 鍵がかかっているかどうかのフラグ
		*/
		const bool IsLocked()const {

			return m_isLocked;
		}

		/**
		 * @brief ドアが開いている？
		 * @return ドアが開いているかどうかのフラグ
		*/
		const bool IsOpened()const {

			return m_isOpened;
		}

		/**
		 * @brief ドアを開けられたら開ける処理を行う関数。
		 * @param rotNum ドアを回転させるための回数
		*/
		void JudgeAndExecuteOpenDoor(unsigned int& rotNum);

		/**
		 * @brief ドアの回転を更新する関数。
		 * @param rotNum ドアを回転させるための回数
		 * @param rotValue ドアの回転の値
		*/
		void UpdateRotation(unsigned int& rotNum, float& rotValue);
	private:
		bool m_isLocked = false;							//鍵がかかっている？
		bool m_isOpened = false;							//開いている？
		CModelRender* m_modelRender = nullptr;				//モデル
		PhysicsStaticObject m_physicsStaticObject;			//物理オブジェクト
		CVector3 m_position = CVector3::Zero;				//座標
		CQuaternion m_rotation = CQuaternion::Identity;		//回転
		CVector3 m_scale = CVector3::One;					//拡大
	};
}

