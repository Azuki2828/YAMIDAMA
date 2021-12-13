#pragma once

namespace nsMyGame {

	//タイトルクラス
	class CGameTitle : public CIGameObject
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
	private:
		CSpriteRender* m_titleSprite = nullptr;		//タイトル画像
	};
}

