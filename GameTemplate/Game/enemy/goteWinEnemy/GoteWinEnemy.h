#pragma once
#include "../Enemy.h"
#include "GoteWinEnemyTriggerBox.h"

namespace nsMyGame {

	namespace nsEnemy {

		//後手必勝のエネミークラス
		class CGoteWinEnemy : public CEnemy
		{
		private:
			//ステート
			enum EnState {
				enState_Idle,
				enState_Walk,
				enState_Attack,
				enState_Guard,
				enState_Damage,
				enState_Death,

				enState_Num
			};

			//アニメーションのリスト
			enum EnAnimationList {
				enAnim_Idle,
				enAnim_Walk,
				enAnim_Attack,
				enAnim_Guard,
				enAnim_Damage,
				enAnim_Death,

				enAnim_Num
			};

			/**
			 * @brief ステータスを初期化する関数。
			*/
			void InitStatus();

			/**
			 * @brief アニメーションクリップを初期化する関数。
			*/
			void InitAnimationClip();

			/**
			 * @brief アニメーションを更新する関数。
			*/
			void AnimationUpdate()override final;
		public:
			/**
			 * @brief Update()関数の前に一度だけ呼ばれる関数。
			 * @return 完了した？
			*/
			bool StartSub()override final;

			/**
			 * @brief 更新関数。
			*/
			void UpdateSub()override final;

			/**
			 * @brief ステートを設定する関数。
			 * @param stateNum ステート番号
			*/
			void ChangeState(int stateNum) {

				m_state = static_cast<EnState>(stateNum);
			}

			/**
			 * @brief 移動関数。
			*/
			void Move()override final;

			/**
			 * @brief トリガーボックスを更新する関数。
			 * @param pos 座標
			 * @param rot 回転
			 * @param forward 前方向
			*/
			void UpdateTriggerBox(const CVector3& pos, const CQuaternion& rot, const CVector3& forward)override final;

			/**
			 * @brief ダメージフラグを設定する関数。
			 * @param receiveDamageFlag ダメージフラグ
			*/
			void SetReceiveDamage(bool receiveDamageFlag)override final {

				if (m_state == enState_Guard) {

					m_player = FindGO<nsPlayer::CPlayer>("player");
					m_player->AttackBreak();

				}
				m_receiveDamage = receiveDamageFlag;

				//ダメージを与える。
				if (m_receiveDamage) {
					m_coolTime = 0.5f;
					m_status.hp -= 70;
					m_state = enState_Damage;
				}
			}
		private:
			EnState m_state = enState_Idle;					//ステート
			CAnimationClip m_animationClip[enAnim_Num];		//アニメーションクリップ
			CGoteWinEnemyTriggerBox m_triggerBox;			//攻撃用のトリガーボックス
		};
	};
}


