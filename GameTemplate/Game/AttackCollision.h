#pragma once

namespace nsMyGame {

	//攻撃用の当たり判定クラス
	class CAttackCollision : public CIGameObject
	{
	public:
		/**
		 * @brief デストラクタ。コリジョンを削除する。
		*/
		~CAttackCollision();

		/**
		 * @brief ボックスの当たり判定を作成する関数。
		*/
		void CreateBox(const CVector3& pos, const CQuaternion& rot, const CVector3& size);

		/**
		 * @brief ワールド行列を設定する関数。
		 * @param worldMatrix ワールド行列
		*/
		void SetWorldMatrix(const CMatrix& matrix) {

			CVector3 position;
			position.x = matrix.m[3][0];
			position.y = matrix.m[3][1];
			position.z = matrix.m[3][2];
			SetPosition(position);

			CQuaternion rotation;
			rotation.SetRotation(matrix);
			SetRotation(rotation);
		}

		/**
		 * @brief 有効タイムを設定する関数。
		 * @param activeTime 有効タイム
		*/
		void SetActiveTime(const float activeTime){

			m_activeTime = activeTime;
		}

		/**
		 * @brief 衝突したコリジョンが自身か？
		 * @param contactObject 当たり判定オブジェクト
		 * @return コリジョンが自身かどうかのフラグ
		*/
		bool IsSelf(const btCollisionObject& contactObject) {

			return m_ghostBox.IsSelf(contactObject);
		}

		/**
		 * @brief ガードフラグを設定する関数。
		 * @param guardFlag ガードフラグ
		*/
		void SetGuardFlag(const bool guardFlag) {

			m_isGuarded = guardFlag;
		}

		/**
		 * @brief ガードされた？
		 * @return ガードされたかどうかのフラグ
		*/
		const bool IsGuarded()const {

			return m_isGuarded;
		}
	private:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

		/**
		 * @brief 座標を設定する関数。
		 * @param pos 座標
		*/
		void SetPosition(const CVector3& pos) {

			m_ghostBox.SetPosition(pos);
		}

		/**
		 * @brief 回転を設定する関数。
		 * @param rot 回転
		*/
		void SetRotation(const CQuaternion& rot) {

			m_ghostBox.SetRotation(rot);
		}
	private:
		float m_activeTime = 0.0f;						//有効タイム
		bool m_isGuarded = false;						//ガードされた？
		CPhysicsGhostObject m_ghostBox;					//トリガーボックス
	};
}

