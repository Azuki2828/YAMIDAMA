#pragma once

namespace nsMyGame {

	//エイリアス宣言
	class CBackGround;
	class CMainCamera;
	class CGameHUD;

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
	public:
		/**
		 * @brief ゲームクリアにする関数。
		*/
		void GameClear() {

			m_isGameClear = true;
		}
	private:
		bool m_isGameClear = false;					//ゲームクリア？
		bool m_gameOverSoundFlag = false;			//ゲームオーバーサウンドが再生された？
		float m_youDiedSpriteTrans = 0.0f;			//死亡スプライトの透明度
		float m_youDiedMessageTime = 0.0f;			//ゲームオーバースプライトがでるまでの時間
		float m_gameClearSpriteTrans = 0.0f;		//ゲームクリアスプライトの透明度
		float m_gameClearMessageTime = 0.0f;		//ゲームクリアスプライトがでるまでの時間

		//各クラスのポインタ変数
		nsPlayer::CPlayer* m_player = nullptr;
		CSpriteRender* m_youDiedSprite = nullptr;
		CSpriteRender* m_gameClearSprite = nullptr;
		CBackGround* m_backGround = nullptr;
		CGameHUD* m_gameHUD = nullptr;
		CMainCamera* m_mainCamera = nullptr;
		CFade* m_fade = nullptr;
	};
}

