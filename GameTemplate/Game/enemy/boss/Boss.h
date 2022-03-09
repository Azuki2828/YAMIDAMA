#pragma once
#include "../constEnemy.h"
#include "../Enemy.h"
#include "BossAttackCollisionDetection.h"
#include "../../AttackCollision.h"
#include "../../GameHUD.h"

namespace nsMyGame {

	namespace nsEnemy {

		//ボスクラス
		class CBoss : public CEnemy
		{
		private:
			//ステート
			enum EnState {
				enState_Idle,
				enState_Walk,
				enState_JumpAttack,
				enState_SwipingAttack,
				enState_Damage,
				enState_Death,
				enState_Scream,
				enState_Start,

				enState_Num
			};

			//アニメーションのリスト
			enum EnAnimationList {
				enAnim_Idle,
				enAnim_Walk,
				enAnim_JumpAttack,
				enAnim_SwipingAttack,
				enAnim_Damage,
				enAnim_Death,
				enAnim_Scream,
				enAnim_Start,

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
			 * @brief 通常移動関数。
			*/
			void MoveCommon();

			/**
			 * @brief ジャンプ攻撃時の移動関数。
			*/
			void JumpAttackMove();

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

					m_status.hp -= 30;

					auto hud = FindGO<CGameHUD>(c_classNameGameHUD);
					hud->UpdateBossHUD(m_status.hp);
				}
			}

			/**
			 * @brief ガードされた？
			 * @returnガードされたかどうかの判定
			*/
			bool IsGuarded()override final {

				return m_triggerBox.IsGuarded();
			}

			/**
			 * @brief プレイヤーに気づいた？
			 * @return プレイヤーに気づいたかどうかの判定
			*/
			const bool NoticePlayer()const {

				return m_noticePlayer;
			}

			/**
			 * @brief プレイヤーに気づくための関数。
			*/
			void FindPlayer();
		private:
			Bone* m_swordBone = nullptr;							//剣に取り付けられたボーン
			EnState m_state = enState_Start;						//ステート
			CAnimationClip m_animationClip[enAnim_Num];				//アニメーションクリップ
			CBossCollisionDetection m_triggerBox;					//攻撃用のトリガーボックス

			bool m_isAnimStart = true;								//最初のアニメーション？
			bool m_canMove = false;									//移動できる？
			bool m_noticePlayer = false;							//プレイヤーに気づいた？
			CPhysicsGhostObject m_noticePlayerTriggerBox;			//プレイヤーに気づくためのトリガー
			CVector3 m_vecToPlayer = CVector3::Zero;				//プレイヤーに伸びるベクトル
		};
	}
}
