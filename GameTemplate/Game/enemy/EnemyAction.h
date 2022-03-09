#pragma once
#include "constEnemy.h"

namespace nsMyGame {

	namespace nsEnemy {

		//エネミーアクションクラス

		class CEnemyAction {

		public:
			/**
			 * @brief 初期化関数。キャラコンの設定を行う。
			 * @param position プレイヤーの座標。
			*/
			void Init(const CVector3& position, CharacterController& charaCon);

			/**
			 * @brief 移動処理を行う関数。
			 * @param position 座標
			 * @param forward 前方向
			 * @param playerState プレイヤーステート
			*/
			//void Move(CVector3& position, CVector3& forward, EnEnemyState& playerState);

			/**
			 * @brief 回転処理を行う関数。
			 * @param rotation 回転
			*/
			void Rotate(const CQuaternion& rotation);

			/**
			 * @brief 更新関数。クールタイムの更新などを行う。
			*/
			void Update();


			
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
		private:
			bool m_isAttack = false;							//アタック中？
			bool m_isDash = false;								//ダッシュ中？
			float m_coolTime = 0.0f;							//クールタイム
			CVector3 m_position = CVector3::Zero;				//座標
			CQuaternion m_rotation = CQuaternion::Identity;		//回転
			CVector3 m_moveSpeed = CVector3::Zero;				//移動速度
			CVector3 m_rollingSpeed = CVector3::Zero;
			CharacterController* m_charaCon;						//キャラクターコントローラー
		};
	}
}