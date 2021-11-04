#pragma once
#include "PlayerAction.h"
#include "PlayerAnimation.h"

namespace nsMyGame {

	namespace nsPlayer {	

		//プレイヤークラス
		class CPlayer : public CIGameObject
		{
			//ステータス
			struct SStatus {
				int hp = 0;
				int attack = 0;
			};
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

			/**
			 * @brief キャラクターコントローラーを取得する関数。
			 * @return キャラクターコントローラー
			*/
			CharacterController& GetCharacterController() {

				return m_playerAction.GetCharacterController();
			}

			/**
			 * @brief ダメージフラグを設定する関数。
			 * @param recieveDamageFlag ダメージフラグ
			*/
			void SetReceiveDamage(bool recieveDamageFlag) {

				m_receiveDamage = recieveDamageFlag;
				if (m_receiveDamage) {
					m_playerAction.ReceiveDamage();
					m_playerState = enState_Damage;
				}
			}

			/**
			 * @brief ダメージフラグを取得する関数。
			 * @return ダメージフラグ
			*/
			const bool GetReceiveDamage()const {

				return m_receiveDamage;
			}
		private:
			bool m_receiveDamage = false;				//ダメージを受けたか？
			SStatus m_status;							//ステータス
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

