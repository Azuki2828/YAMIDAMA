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
		bool m_finishedFadeOut = false;				//フェードアウトが終わった？
		CSpriteRender* m_titleSprite = nullptr;		//タイトル画像
		CSpriteRender* m_newGameSprite = nullptr;	//「NEW GAME」テキスト
		CSpriteRender* m_textBackSprite = nullptr;	//テキストの後ろに配置する画像
		CFade* m_fade = nullptr;					//フェード
	};
}

