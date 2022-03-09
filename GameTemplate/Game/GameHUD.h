#pragma once
#include "enemy/constEnemy.h"

namespace nsMyGame {

	//HUDクラス
	class CGameHUD : public CIGameObject
	{
		//赤HPバーの減少の仕方
		enum EnHPDecreaseType {

			enType_Linear,		//線形
			enType_NonLinear	//非線形
		};

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

		/**
		 * @brief 赤HPバーの更新を行う関数。
		*/
		void UpdateRedHPBar();
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

			if (m_bossHpBarScale.x <= 0.0f) {

				m_bossHpBarScale.x = 0.0f;
			}
			//拡大率を設定。
			m_bossHpBar->SetScale(m_bossHpBarScale);
		}
	private:
		CSpriteRender* m_hpBarFrame = nullptr;			//HPバーのフレーム
		CSpriteRender* m_hpBar_Green = nullptr;			//HPバー(緑)
		CSpriteRender* m_hpBar_Red = nullptr;			//HPバー(赤)
		CSpriteRender* m_hpBarBack = nullptr;			//HPバーの背景
		CVector3 m_hpBarScale = CVector3::One;			//HPバーの拡大率

		bool m_isActiveBossHPBar = false;				//ボスのHPバーが有効？
		CSpriteRender* m_bossHpBarFrame = nullptr;		//ボスのHPバーのフレーム
		CSpriteRender* m_bossHpBar = nullptr;			//ボスのHPバー
		CSpriteRender* m_bossHpBarBack = nullptr;		//ボスのHPバーの背景
		CVector3 m_bossHpBarScale = CVector3::One;		//ボスのHPバーの拡大率
		nsFont::CFontRender* m_bossNameText = nullptr;	//ボスの名前

		EnHPDecreaseType m_hpDecreaseType = enType_Linear;	//赤HPバーの減少の仕方
		bool m_canHPBarScaleUpdate = true;					//赤HPバーの拡大率を更新できる？
		float m_damageTimer = 0.0f;							//赤HPゲージが減少するまでの時間をはかるタイマー
		float m_damageScale = 0.0f;							//赤HPゲージの減少の拡大率
	};
}

