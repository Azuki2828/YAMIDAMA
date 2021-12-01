#pragma once
#include "PlayerAction.h"
#include "PlayerAnimation.h"
#include "PlayerSelect.h"

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
			 * @return 座標
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
			void SetReceiveDamage(const bool recieveDamageFlag) {

				//ダメージフラグを設定。
				m_receiveDamage = recieveDamageFlag;
				if (!m_receiveDamage) {
					return;
				}

				//ローリング状態ならダメージを食らわない。
				if (m_playerState == enState_Rolling) {
					return;
				}
				//ガードしたならガード成功状態に。
				else if (m_playerState == enState_Guard) {

					m_playerState = enState_GuardSuccess;
					m_playerAction.GuardSuccess();
					return;
				}
				//ダメージを受けたならダメージ状態にする。
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

			void AttackBreak() {

				m_playerState = enState_AttackBreak;
				m_playerAction.AttackBreak();
			}

			/**
			 * @brief ガード中？
			 * @return ガードしているかどうかのフラグ
			*/
			bool IsGuard() {

				return m_playerAction.IsGuard();
			}

			/**
			 * @brief 鍵を所持している？
			 * @return 鍵の所持数が０より大きいかどうかの判定
			*/
			const bool HasKey()const {

				return m_hasKeyNum > 0;
			}

			/**
			 * @brief 鍵を取得する関数。
			*/
			void GetKey() {

				m_hasKeyNum++;
			}

			/**
			 * @brief 鍵を消費する関数。
			*/
			void ConsumeKey() {

				m_hasKeyNum--;
			}

			/**
			 * @brief 何かを選んでいるかどうかのフラグを設定する関数。
			 * @param selectFlag 何かを選んでいるかどうかのフラグ
			*/
			void SetSelectFlag(const bool selectFlag) {

				m_isSelect = selectFlag;
			}

			/**
			 * @brief 何かを選んでいる状態？
			 * @return 何かを選んでいるかどうかのフラグ
			*/
			const bool IsSelect()const {

				return m_isSelect;
			}

			/**
			 * @brief オブジェクトを選択状態にする関数。
			 * @param selectObj オブジェクト
			*/
			void SetSelectObject(CIGameObject* selectObj) {

				m_playerSelect.SetSelectObject(selectObj, m_position);
			}
		private:
			bool m_receiveDamage = false;				//ダメージを受けたか？
			bool m_isSelect = false;					//何かを選んでいる状態？（近くの何かに反応している？）
			int m_hasKeyNum = 0;						//鍵の所持数
			SStatus m_status;							//ステータス
			CModelRender* m_modelRender = nullptr;		//モデル
			CPlayerAction m_playerAction;				//プレイヤーアクションクラス
			CPlayerAnimation m_playerAnimation;			//プレイヤーアニメーションクラス
			CPlayerSelect m_playerSelect;				//プレイヤーの選択状態クラス
			CVector3 m_position = CVector3::Zero;		//座標
			CQuaternion m_rotation;						//回転
			CVector3 m_forward = CVector3::Zero;		//前方向
			EnPlayerState m_playerState;				//プレイヤーのステート
		};
	}
}

