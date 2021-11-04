#pragma once
#include "PlayerTriggerBox.h"

namespace nsMyGame {

	namespace nsPlayer {

		//プレイヤーアクションクラス

		class CPlayerAction
		{
		public:
			/**
			 * @brief 初期化関数。
			 * @param position プレイヤーの座標。
			*/
			void Init(const CVector3& position, const CQuaternion& rotation, const CVector3& forward);

			/**
			 * @brief 移動処理を行う関数。
			 * @param position 座標
			 * @param forward 前方向
			 * @param playerState プレイヤーステート
			*/
			void Move(CVector3& position, CVector3& forward, EnPlayerState& playerState);

			/**
			 * @brief 回転処理を行う関数。
			 * @param rotation 回転
			*/
			void Rotate(CQuaternion& rotation);

			/**
			 * @brief アクションを行う関数。
			 * @param playerState プレイヤーステート
			*/
			void Action(EnPlayerState& playerState);

			/**
			 * @brief 更新関数。クールタイムの更新などを行う。
			*/
			void Update(const CVector3& pos, const CQuaternion& rot,const CVector3& forward, EnPlayerState& playerState);

			/**
			 * @brief キャラクターコントローラーを取得する関数。
			 * @return キャラクターコントローラー
			*/
			CharacterController& GetCharacterController() {

				return m_charaCon;
			}
		private:
			/**
			 * @brief クールタイム中かどうか判定する関数。
			 * @return クールタイム中？
			*/
			const bool IsCoolTime()const {

				return m_coolTime > 0.0f;
			}

			/**
			 * @brief ダッシュ中かどうか判定する関数。
			 * @return ダッシュ中？
			*/
			const bool IsDash()const {

				return m_isDash;
			}

			const bool GetHitAttackFlag()const {

				return m_hitAttack;
			}

			void SetHitAttackFlag(bool hitAttackFlag) {

				m_hitAttack = hitAttackFlag;
			}

			
		private:
			bool m_isAttack = false;							//アタック中？
			bool m_hitAttack = false;							//攻撃が当たった？
			bool m_isDash = false;								//ダッシュ中？
			float m_coolTime = 0.0f;							//クールタイム
			CVector3 m_position = CVector3::Zero;				//座標
			CQuaternion m_rotation = CQuaternion::Identity;		//回転
			CVector3 m_moveSpeed = CVector3::Zero;				//移動速度
			CVector3 m_rollingSpeed = CVector3::Zero;
			CharacterController m_charaCon;						//キャラクターコントローラー
			CPlayerTriggerBox m_triggerBox;						//攻撃時に使用するトリガーボックス
		};
	}
}
