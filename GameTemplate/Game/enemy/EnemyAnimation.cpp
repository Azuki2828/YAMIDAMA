#include "stdafx.h"
#include "EnemyAnimation.h"

namespace nsMyGame {

	namespace nsEnemy {

		void CEnemyAnimation::Init() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/idle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Run].Load("Assets/animData/run.tka");
			m_animationClip[enAnim_Run].SetLoopFlag(true);
			m_animationClip[enAnim_Attack].Load("Assets/animData/attack.tka");
			m_animationClip[enAnim_Attack].SetLoopFlag(false);
			m_animationClip[enAnim_Rolling].Load("Assets/animData/rolling.tka");
			m_animationClip[enAnim_Rolling].SetLoopFlag(false);
			m_animationClip[enAnim_ThreeCombo].Load("Assets/animData/threeCombo.tka");
			m_animationClip[enAnim_ThreeCombo].SetLoopFlag(false);
		}

		void CEnemyAnimation::Update(CModelRender& modelRender, const EnEnemyState& enemyState) {

			//敵のステートに応じてアニメーションを再生。
			switch (enemyState) {
			case enState_Idle:
				modelRender.PlayAnimation(enAnim_Idle, 0.4f);
				break;
			case enState_Attack:
				modelRender.PlayAnimation(enAnim_Attack, 0.4f);
				break;
			}
		}
	}
}