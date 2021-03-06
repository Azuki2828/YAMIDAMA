#pragma once
#include "../constEnemy.h"
#include "../Enemy.h"
#include "FirstWinEnemyAttackCollisionDetection.h"
#include "../../AttackCollision.h"

namespace nsMyGame {

	namespace nsEnemy {
		 
		class CFirstWinEnemy : public CEnemy
		{
		private:
			//ステート
			enum EnState {
				enState_Idle,
				enState_Walk,
				enState_ThreeCombo,
				enState_Damage,
				enState_Death,
				enState_AttackBreak,
				enState_Back,

				enState_Num
			};

			//アニメーションのリスト
			enum EnAnimationList {
				enAnim_Idle,
				enAnim_Walk,
				enAnim_ThreeCombo,
				enAnim_Damage,
				enAnim_Death,
				enAnim_AttackBreak,
				enAnim_Back,

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

			/**
			 * @brief アニメーションイベント用の関数。
			 * @param clipName アニメーションの名前
			 * @param eventName アニメーションイベントのキーの名前
			*/
			void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

			/**
			 * @brief 攻撃用の当たり判定を作成する関数。
			*/
			void CreateAttackCollision();
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
			void UpdateTriggerBox()override final;

			/**
			 * @brief ダメージフラグを設定する関数。
			 * @param receiveDamageFlag ダメージフラグ
			*/
			void SetReceiveDamage(const bool receiveDamageFlag)override final {

				m_receiveDamage = receiveDamageFlag;

				//ダメージを与える。
				if (m_receiveDamage) {
					m_coolTime = 0.3f;
					m_status.hp -= 30;
					m_state = enState_Damage;

					//攻撃判定を無効化。
					m_triggerBox.Deactivate();
				}
			}

			/**
			 * @brief ガードされた？
			 * @return
			*/
			bool IsGuarded()override final {

				return m_triggerBox.IsGuarded();
			}
		private:
			Bone* m_swordBone = nullptr;							//剣に取り付けられたボーン
			EnState m_state = enState_Idle;							//ステート
			CAnimationClip m_animationClip[enAnim_Num];				//アニメーションクリップ
			CFirstWinEnemyAttackCollisionDetection m_triggerBox;	//攻撃用のトリガーボックス
		};
	}
}
