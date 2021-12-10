#pragma once
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
			void UpdateTriggerBox(const CVector3& pos, const CQuaternion& rot, const CVector3& forward)override final;

			/**
			 * @brief ダメージフラグを設定する関数。
			 * @param receiveDamageFlag ダメージフラグ
			*/
			void SetReceiveDamage(const bool receiveDamageFlag)override final {

				m_receiveDamage = receiveDamageFlag;

				//ダメージを与える。
				if (m_receiveDamage) {
					m_coolTime = 0.5f;
					m_status.hp -= 70;
					m_state = enState_Damage;
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
			bool m_isAttack = false;								//攻撃中？
			int m_swordBoneNum = 0;									//剣に取り付けられたボーンの番号
			EnState m_state = enState_Idle;							//ステート
			CAnimationClip m_animationClip[enAnim_Num];				//アニメーションクリップ
			CFirstWinEnemyAttackCollisionDetection m_triggerBox;	//攻撃用のトリガーボックス
			std::vector<CAttackCollision*> m_attackCollision;			//攻撃用当たり判定のリスト
		};
	}
}
