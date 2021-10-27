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

				enState_Num
			};

			//アニメーションのリスト
			enum EnAnimationList {
				enAnim_Idle,
				enAnim_Walk,
				enAnim_ThreeCombo,

				enAnim_Num
			};

			void InitAnimationClip();

			void AnimationUpdate()override final;
		public:
			bool StartSub()override final;
			void UpdateSub()override final;

			void ChangeState(int stateNum) {

				m_state = static_cast<EnState>(stateNum);
			}

			void Move()override final;
		private:
			EnState m_state = enState_Idle;
			CAnimationClip m_animationClip[enAnim_Num];
		};
	}
}
