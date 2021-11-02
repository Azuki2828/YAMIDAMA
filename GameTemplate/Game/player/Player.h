#pragma once
#include "PlayerAction.h"
#include "PlayerAnimation.h"

namespace nsMyGame {

	namespace nsPlayer {	

		//プレイヤークラス

		class CPlayer : public CIGameObject
		{
			
			/**
			 * @brief ライトカメラを更新する関数。
			*/
			void LightCameraUpdate();

			/**
			 * @brief 最初に一回だけ呼ばれる関数。
			 * @return 成功した？
			*/
			bool Start()override final;

			/**
			 * @brief 更新関数。
			*/
			void Update()override final;

			/**
			 * @brief プレイヤーの前方向を更新する関数。
			*/
			void UpdateForward();
		public:
			/**
			 * @brief 座標を取得する関数。
			 * @return
			*/
			const CVector3& GetPosition()const {

				return m_position;
			}

			/**
			 * @brief モデルレンダーのインスタンスを取得する関数。
			 * @return モデルレンダーのインスタンス
			*/
			CModelRender* GetModelRender() {

				return m_modelRender;
			}

			CharacterController* GetCharacterController() {

				return m_playerAction.GetCharacterController();
			}

			void SetRecieveDamage(bool recieveDamageFlag) {

				m_recieveDamage = recieveDamageFlag;
			}

			bool GetRecieveDamage() {

				return m_recieveDamage;
			}
		private:
			bool m_recieveDamage = false;
			CModelRender* m_modelRender = nullptr;		//モデル
			CPlayerAction m_playerAction;				//プレイヤーの行動をまとめたクラス
			CPlayerAnimation m_playerAnimation;			//プレイヤーアニメーション
			CVector3 m_position = CVector3::Zero;		//座標
			CQuaternion m_rotation;						//回転
			CVector3 m_forward = CVector3::Zero;		//プレイヤーの前方向
			EnPlayerState m_playerState;				//プレイヤーのステート
		};
	}
}

