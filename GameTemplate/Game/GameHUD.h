#pragma once

namespace nsMyGame {

	//HUDクラス
	class CGameHUD : public CIGameObject
	{
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 削除関数。
		*/
		void OnDestroy()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

		/**
		 * @brief HPバーの更新を行う関数。
		*/
		void UpdateHPBar();
	private:
		CSpriteRender* m_hpBarFrame = nullptr;			//HPバーのフレーム
		CSpriteRender* m_hpBar = nullptr;				//HPバー
		CSpriteRender* m_hpBarBack = nullptr;			//HPバーの背景
		CVector3 m_hpBarScale = CVector3::One;			//HPバーの拡大率
	};
}

