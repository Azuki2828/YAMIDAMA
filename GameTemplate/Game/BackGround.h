#pragma once

namespace nsMyGame {

	//エイリアス宣言
	class CDoor;

	namespace nsEnemy {

		class CGoteWinEnemy;
		class CFirstWinEnemy;
		class CBoss;
	}
	namespace nsItem {

		class CItem;
	}

	//背景クラス
	class CBackGround : public CIGameObject
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

		/**
		 * @brief ステージをロードする関数。
		*/
		void LoadStage();

		/**
		 * @brief プレイヤーの選択状態を更新する関数。
		*/
		void UpdatePlayerSelect();

		/**
		 * @brief 松明の炎エフェクトの更新関数。
		*/
		void UpdateFireEffect();
	public:
		/**
		 * @brief 太陽光用のディレクションライトを作成する関数。
		*/
		static void CreateDirLight() {

			m_dirLight[0] = new nsLight::CDirectionLight;

			m_dirLight[0] = NewGO<nsLight::CDirectionLight>(enPriority_Zeroth);
			m_dirLight[0]->SetLigDirection({ 1.0f,-1.0f,-1.0f });
			m_dirLight[0]->SetLigColor({ 1.0f,1.0f,1.0f });

		
			m_dirLight[1] = new nsLight::CDirectionLight;

			m_dirLight[1] = NewGO<nsLight::CDirectionLight>(enPriority_Zeroth);
			m_dirLight[1]->SetLigDirection(g_camera3D->GetForward());
			m_dirLight[1]->SetLigColor({ 0.0f,0.0f,0.0f });
		}

		static void RemoveDirLight() {

			DeleteGO(m_dirLight[0]);
			DeleteGO(m_dirLight[1]);
		}

		/**
		 * @brief 太陽光の方向を取得する関数。
		 * @return 太陽光の方向
		*/
		static nsLight::CDirectionLight* GetSunDirectionLight() {

			return m_dirLight[0];
		}

		/**
		 * @brief ボスが登場した？
		 * @return ボスが登場したかどうかのフラグ
		*/
		const bool CreatedBoss()const {

			return m_createBoss;
		}
	private:
		float m_fireTime = 0.0f;							//炎を発生させるための時間
		CModelRender* m_modelRender = nullptr;				//モデル
		std::vector<CDoor*> m_door;							//ドアクラスのインスタンス
		std::vector<nsEnemy::CFirstWinEnemy*> m_fWEnemy;	//先手必勝の敵のインスタンス
		std::vector<nsEnemy::CGoteWinEnemy*> m_gWEnemy;		//後手必勝の敵のインスタンス
		nsEnemy::CBoss* m_boss = nullptr;					//ボスのインスタンス
		std::vector<nsLight::CPointLight*> m_pointLight;	//ポイントライトのインスタンス
		std::vector<nsItem::CItem*> m_item;					//アイテムのインスタンス
		std::vector<Effect*> m_fireEffect;					//炎エフェクトのインスタンス
		static nsLight::CDirectionLight* m_dirLight[2];		//ディレクションライト
		CVector3 m_pos = CVector3::Zero;					//座標
		CQuaternion m_rot = CQuaternion::Identity;			//回転
		CVector3 m_sca = CVector3::One;						//拡大
		Level m_level;										//レベル
		PhysicsStaticObject m_physicsStaticObject;			//静的物理オブジェクト
		CPhysicsGhostObject m_noticePlayerTriggerBox;			//ボスが登場するためのトリガー
		CVector3 m_bossPosition = CVector3::Zero;				//ボスの登場座標
		CQuaternion m_bossRotation = CQuaternion::Identity;		//ボスの登場回転
		bool m_createBoss = false;								//ボスが登場した？

		//各インスタンスの数
		int m_doorNum = 0;
		int m_fEnemyNum = 0;
		int m_gEnemyNum = 0;
		int m_pointLightNum = 0;
		int m_fireEffectNum = 0;
		int m_itemNum = 0;
	};
}

