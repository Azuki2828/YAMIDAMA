#include "stdafx.h"
#include "PlayerAnimation.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerAnimation::Init(CModelRender& modelRender) {

			//モデルからスケルトンのファイルパスを取得し、スケルトンを構築。
			m_skeleton[enAnimation_Base].Init(modelRender.GetFilePathTks());

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/idle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Run].Load("Assets/animData/run.tka");
			m_animationClip[enAnim_Run].SetLoopFlag(true);
			m_animationClip[enAnim_Attack].Load("Assets/animData/attack.tka");
			m_animationClip[enAnim_Attack].SetLoopFlag(false);
			m_animationClip[enAnim_AttackBreak].Load("Assets/animData/attackBreak.tka");
			m_animationClip[enAnim_AttackBreak].SetLoopFlag(false);
			m_animationClip[enAnim_Damage].Load("Assets/animData/damage.tka");
			m_animationClip[enAnim_Damage].SetLoopFlag(false);
			m_animationClip[enAnim_Rolling].Load("Assets/animData/rolling.tka");
			m_animationClip[enAnim_Rolling].SetLoopFlag(false);
			m_animationClip[enAnim_Guard].Load("Assets/animData/guard.tka");
			m_animationClip[enAnim_Guard].SetLoopFlag(false);
			m_animationClip[enAnim_GuardSuccess].Load("Assets/animData/guardSuccess.tka");
			m_animationClip[enAnim_GuardSuccess].SetLoopFlag(false);
			m_animationClip[enAnim_Death].Load("Assets/animData/death.tka");
			m_animationClip[enAnim_Death].SetLoopFlag(false);
			m_animationClip[enAnim_LeftWalk].Load("Assets/animData/leftWalk.tka");
			m_animationClip[enAnim_LeftWalk].SetLoopFlag(true);
			m_animationClip[enAnim_RightWalk].Load("Assets/animData/rightWalk.tka");
			m_animationClip[enAnim_RightWalk].SetLoopFlag(true);

			//ベースとなるアニメーションを初期化。
			m_animation[enAnimation_Base].Init(m_skeleton[enAnimation_Base], m_animationClip, enAnim_Num);

			//このモデルはアニメーションを再生する。
			modelRender.SetAnimFlg(true);

			modelRender.GiveUniqueAnimation();
		}

		void CPlayerAnimation::Update(CModelRender& modelRender, const EnPlayerState& playerState) {


			//スケルトンを更新。
			m_skeleton[enAnimation_Base].Update(modelRender.GetModel()->GetWorldMatrix());

			//プレイヤーステートに応じてアニメーションを再生。

			switch (playerState) {
			case enState_Idle:
				m_animation[enAnimation_Base].Play(enAnim_Idle, 0.8f);
				break;
			case enState_Walk:
				m_animation[enAnimation_Base].Play(enAnim_Walk, 0.8f);
				break;
			case enState_LeftWalk:
				m_animation[enAnimation_Base].Play(enAnim_LeftWalk, 0.8f);
				break;
			case enState_RightWalk:
				m_animation[enAnimation_Base].Play(enAnim_RightWalk, 0.8f);
				break;
			case enState_Run:
				m_animation[enAnimation_Base].Play(enAnim_Run, 0.8f);
				break;
			case enState_Attack:
				m_animation[enAnimation_Base].Play(enAnim_Attack, 0.8f);
				break;
			case enState_AttackBreak:
				m_animation[enAnimation_Base].Play(enAnim_AttackBreak, 0.8f);
				break;
			case enState_Damage:
				m_animation[enAnimation_Base].Play(enAnim_Damage, 0.4f);
				break;
			case enState_Rolling:
				m_animation[enAnimation_Base].Play(enAnim_Rolling, 0.8f);
				break;
			case enState_Guard:
				m_animation[enAnimation_Base].Play(enAnim_Guard, 0.4f);
				break;
			case enState_GuardSuccess:
				m_animation[enAnimation_Base].Play(enAnim_GuardSuccess, 0.1f);
				break;
			case enState_Death:
				m_animation[enAnimation_Base].Play(enAnim_Death, 0.8f);
				break;
			}

			//アニメーションを進める。
			m_animation[enAnimation_Base].Progress(m_animationSpeed / 30.0f);

			//スケルトンをモデルレンダーのスケルトンにコピー。
			modelRender.CopyBone(m_skeleton[enAnimation_Base]);
		}
	}
}