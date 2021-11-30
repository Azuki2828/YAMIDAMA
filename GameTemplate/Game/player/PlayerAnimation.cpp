#include "stdafx.h"
#include "PlayerAnimation.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerAnimation::Init() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/walk2.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/idle2.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Run].Load("Assets/animData/run2.tka");
			m_animationClip[enAnim_Run].SetLoopFlag(true);
			m_animationClip[enAnim_Attack].Load("Assets/animData/attack2.tka");
			m_animationClip[enAnim_Attack].SetLoopFlag(false);
			m_animationClip[enAnim_AttackBreak].Load("Assets/animData/attackBreak.tka");
			m_animationClip[enAnim_AttackBreak].SetLoopFlag(false);
			m_animationClip[enAnim_Damage].Load("Assets/animData/damage.tka");
			m_animationClip[enAnim_Damage].SetLoopFlag(false);
			m_animationClip[enAnim_Rolling].Load("Assets/animData/rolling2.tka");
			m_animationClip[enAnim_Rolling].SetLoopFlag(false);
			m_animationClip[enAnim_ThreeCombo].Load("Assets/animData/threeCombo.tka");
			m_animationClip[enAnim_ThreeCombo].SetLoopFlag(false);
			m_animationClip[enAnim_Guard].Load("Assets/animData/guard2.tka");
			m_animationClip[enAnim_Guard].SetLoopFlag(false);
			m_animationClip[enAnim_GuardSuccess].Load("Assets/animData/guardSuccess.tka");
			m_animationClip[enAnim_GuardSuccess].SetLoopFlag(false);
		}

		void CPlayerAnimation::Update(CModelRender& modelRender, const EnPlayerState& playerState) {

			//プレイヤーステートに応じてアニメーションを再生。

			switch (playerState) {
			case enState_Idle:
				modelRender.PlayAnimation(enAnim_Idle, 0.8f);
				break;
			case enState_Walk:
				modelRender.PlayAnimation(enAnim_Walk, 0.8f);
				break;
			case enState_Run:
				modelRender.PlayAnimation(enAnim_Run, 0.8f);
				break;
			case enState_Attack:
				modelRender.PlayAnimation(enAnim_Attack, 0.8f);
				break;
			case enState_AttackBreak:
				modelRender.PlayAnimation(enAnim_AttackBreak, 0.8f);
				break;
			case enState_Damage:
				modelRender.PlayAnimation(enAnim_Damage, 0.8f);
				break;
			case enState_Rolling:
				modelRender.PlayAnimation(enAnim_Rolling, 0.8f);
				break;
			case enState_ThreeCombo:
				modelRender.PlayAnimation(enAnim_ThreeCombo, 0.8f);
				break;
			case enState_Guard:
				modelRender.PlayAnimation(enAnim_Guard, 0.4f);
				break;
			case enState_GuardSuccess:
				modelRender.PlayAnimation(enAnim_GuardSuccess, 0.1f);
				break;
			}
		}
	}
}