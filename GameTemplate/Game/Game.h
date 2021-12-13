#pragma once

namespace nsMyGame {

	//エイリアス宣言
	class CBackGround;
	class CMainCamera;

	namespace nsPlayer {

		class CPlayer;
	}

	//ゲームメインクラス
	class CGameMain : public CIGameObject
	{
	private:
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
		float m_youDiedSpriteTrans = 0.0f;			//死亡スプライトの透明度

		//各クラスのポインタ変数
		nsPlayer::CPlayer* m_player = nullptr;
		CSpriteRender* m_youDiedSprite = nullptr;
		CBackGround* m_backGround = nullptr;
		CMainCamera* m_mainCamera = nullptr;
		Fade* m_fade = nullptr;
	};
}

