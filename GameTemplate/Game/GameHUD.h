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
		 * @brief HPバーを初期化する関数。
		*/
		void InitHPBar();

		/**
		 * @brief HPバーの更新を行う関数。
		*/
		void UpdateHPBar();
	public:
		/**
		 * @brief ボスのHPバーを有効にする関数。
		*/
		void ActivateBossHUD() {

			m_bossHpBarFrame->Activate();
			m_bossHpBar->Activate();
			m_bossHpBarBack->Activate();
			m_bossNameText->Activate();
		}

		void UpdateBossHUD(const int hp) {

			//HPゲージを更新。
			m_bossHpBarScale.x = static_cast<float>(hp) / static_cast<float>(nsEnemy::c_bossMaxHP);

			//拡大率を設定。
			m_bossHpBar->SetScale(m_hpBarScale);
		}
	private:
		CSpriteRender* m_hpBarFrame = nullptr;			//HPバーのフレーム
		CSpriteRender* m_hpBar = nullptr;				//HPバー
		CSpriteRender* m_hpBarBack = nullptr;			//HPバーの背景
		CVector3 m_hpBarScale = CVector3::One;			//HPバーの拡大率

		bool m_isActiveBossHPBar = false;				//ボスのHPバーが有効？
		CSpriteRender* m_bossHpBarFrame = nullptr;		//ボスのHPバーのフレーム
		CSpriteRender* m_bossHpBar = nullptr;			//ボスのHPバー
		CSpriteRender* m_bossHpBarBack = nullptr;		//ボスのHPバーの背景
		CVector3 m_bossHpBarScale = CVector3::One;		//ボスのHPバーの拡大率
		nsFont::CFontRender* m_bossNameText = nullptr;	//ボスの名前
	};
}

