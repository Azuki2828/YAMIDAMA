#include "stdafx.h"
#include "PlayerAnimation.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerAnimation::Init() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/playerWalk.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/playerIdle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Run].Load("Assets/animData/playerRun.tka");
			m_animationClip[enAnim_Run].SetLoopFlag(true);
			m_animationClip[enAnim_Attack].Load("Assets/animData/playerAttack.tka");
			m_animationClip[enAnim_Attack].SetLoopFlag(false);
			m_animationClip[enAnim_Rolling].Load("Assets/animData/playerRolling.tka");
			m_animationClip[enAnim_Rolling].SetLoopFlag(false);
			m_animationClip[enAnim_ThreeCombo].Load("Assets/animData/ThreeCombo.tka");
			m_animationClip[enAnim_ThreeCombo].SetLoopFlag(false);
		}

		void CPlayerAnimation::Update(CModelRender& modelRender, const EnPlayerState& playerState) {

			//プレイヤーステートに応じてアニメーションを再生。
			switch (playerState) {
			case enAnim_Idle:
				modelRender.PlayAnimation(enAnim_Idle, 0.4f);
				break;
			case enAnim_Walk:
				modelRender.PlayAnimation(enAnim_Walk, 0.4f);
				break;
			case enAnim_Run:
				modelRender.PlayAnimation(enAnim_Run, 0.4f);
				break;
			case enAnim_Attack:
				modelRender.PlayAnimation(enAnim_Attack, 0.4f);
				break;
			case enAnim_Rolling:
				modelRender.PlayAnimation(enAnim_Rolling, 0.4f);
				break;
			case enAnim_ThreeCombo:
				modelRender.PlayAnimation(enAnim_ThreeCombo, 0.4f);
				break;
			}
		}
	}
}