#pragma once

namespace nsMyGame {

	class CBackGroundObject : public CIGameObject
	{
	private:
		/**
		 * @brief 削除関数。
		*/
		void OnDestroy()override final;
	public:
		/**
		 * @brief 初期化関数。
		 * @param fileName ファイルパス
		 * @param position 座標
		 * @param rotation 回転
		 * @param scale 拡大
		 * @param isCollision 当たり判定を取る？
		*/
		void InitModel(const char* fileName,
			const CVector3& position,
			const CQuaternion& rotation,
			const CVector3& scale, 
			const bool isCollision = true
		);

		/**
		 * @brief モデルを無視する？
		*/
		void IgnoreObj() {

			m_isIgnoreObject = true;
		}

	private:
		bool m_isIgnoreObject = false;				//モデルを無視する？
		CModelRender* m_modelRender = nullptr;		//モデル
		PhysicsStaticObject m_physicsStaticObject;	//コリジョン
	};
}

