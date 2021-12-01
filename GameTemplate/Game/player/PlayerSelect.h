#pragma once

namespace nsMyGame {

	//プレイヤーの選択状態クラス
	class CPlayerSelect
	{
	public:
		/**
		 * @brief オブジェクトを選択状態にする関数。
		 * @param selectObj オブジェクト
		*/
		void SetSelectObject(CIGameObject* selectObj, const CVector3& playerPos);

		/**
		 * @brief 現在選択中のオブジェクトとプレイヤーとの距離を取得する関数。
		 * @return 現在選択中のオブジェクトとプレイヤーとの距離
		*/
		const float GetSelectObjectLengthToPlayer(const CVector3& playerPos);

		/**
		 * @brief 現在選択中のオブジェクトを更新する関数。
		*/
		void UpdateCarentSelectObject(const CVector3& playerPos);
	private:
		CIGameObject* m_selectObject = nullptr;				//選択状態のオブジェクト
	};
}

