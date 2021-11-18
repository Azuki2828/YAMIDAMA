#pragma once
#include "Door.h"
#include "enemy/firstWinEnemy/FirstWinEnemy.h"
#include "enemy/goteWinEnemy/GoteWinEnemy.h"

namespace nsMyGame {

	class CBackGround : public CIGameObject
	{
	public:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;

		static void CreateDirLight() {

			m_dirLight = new nsLight::CDirectionLight;

			m_dirLight = NewGO<nsLight::CDirectionLight>(enPriority_Zeroth);
			m_dirLight->SetLigDirection({ 1.0f,-1.0f,-1.0f });
			m_dirLight->SetLigColor({ 2.8f,2.8f,2.8f });
		}

		static nsLight::CDirectionLight* GetDirectionLight() {

			return m_dirLight;
		}
	private:
		CModelRender* m_modelRender = nullptr;			//モデル
		CModelRender* m_wall = nullptr;					//壁モデル
		std::vector<CDoor*> m_door;						//ドアクラスのインスタンス
		std::vector<nsEnemy::CFirstWinEnemy*> m_fWEnemy;					//先手必勝の敵のインスタンス
		std::vector<nsEnemy::CGoteWinEnemy*> m_gWEnemy;						//後手必勝の敵のインスタンス
		std::vector<nsLight::CPointLight*> m_pointLight;					//ポイントライトのインスタンス
		static nsLight::CDirectionLight* m_dirLight;	//ディレクションライト
		CVector3 m_pos = CVector3::Zero;				//座標
		CQuaternion m_rot = CQuaternion::Identity;		//回転
		CVector3 m_sca = CVector3::One;					//拡大
		Level m_level;									//レベル
		PhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクト
	};
}

