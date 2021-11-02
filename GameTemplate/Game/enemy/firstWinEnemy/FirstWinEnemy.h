#pragma once
#include "../Enemy.h"

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

				enState_Num
			};

			//アニメーションのリスト
			enum EnAnimationList {
				enAnim_Idle,
				enAnim_Walk,
				enAnim_ThreeCombo,
				enAnim_Damage,
				enAnim_Death,

				enAnim_Num
			};

			void InitStatus();

			void InitAnimationClip();

			void AnimationUpdate()override final;
		public:
			bool StartSub()override final;
			void UpdateSub()override final;

			void ChangeState(int stateNum) {

				m_state = static_cast<EnState>(stateNum);
			}

			void Move()override final;

			
			void SetReceiveDamage(bool receiveDamageFlag)override final {

				m_receiveDamage = receiveDamageFlag;

				//ダメージを与える。
				if (m_receiveDamage) {
					m_coolTime = 0.5f;
					m_status.hp -= 70;
					m_state = enState_Damage;
				}
			}
		private:
			EnState m_state = enState_Idle;
			CAnimationClip m_animationClip[enAnim_Num];
		};
	}
}
