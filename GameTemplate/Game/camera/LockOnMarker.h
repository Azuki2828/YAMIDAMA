#pragma once

namespace nsMyGame {

	//ロックオンアイコンクラス
	class CLockOnMarker
	{
	public:
		/**
		 * @brief 初期化関数。
		*/
		void Init();

		/**
		 * @brief 削除関数。
		*/
		void Release();

		/**
		 * @brief 更新関数。
		*/
		void UpdateMarker(const CVector2& position);
	private:
		/**
		 * @brief 拡大率を更新する関数。
		*/
		void UpdateScale();
	private:
		CSpriteRender* m_lockOnMarker = nullptr;	//ロックオンアイコン
		CVector3 m_position = CVector3::Zero;		//座標
		float m_scale = 1.0f;						//拡大
		short m_sizeStep = 0;						//拡大率変動の段階
		float m_scaleDiff = 0.0f;					//拡大率変動の値
		short m_updateNum = 0;						//拡大率更新回数。

		static const float m_stepScale[];			//段階ごとの目標となる拡大率
	};
}

