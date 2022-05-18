#pragma once
#include "constPlayer.h"

namespace nsMyGame {

	namespace nsPlayer {

		//アニメーションのリスト
		enum EnAnimationList {
			enAnim_Idle,
			enAnim_Walk,
			enAnim_LeftWalk,
			enAnim_RightWalk,
			enAnim_Run,
			enAnim_Attack,
			enAnim_AttackBreak,
			enAnim_Damage,
			enAnim_Rolling,
			enAnim_Guard,
			enAnim_GuardSuccess,
			enAnim_Death,

			enAnim_Num

		};

		//アニメーションブレンドに使用するアニメーションのリスト
		enum EnAnimationBlendNum {

			enAnimation_Base,
			enAnimation_Blend1,

			enAnimation_BlendNum
		};

		//プレイヤーのアニメーションを扱うクラス
		class CPlayerAnimation
		{
		public:
			/**
			 * @brief 初期化関数。
			*/
			void Init(CModelRender& modelRender);

			/**
			 * @brief 更新関数。
			*/
			void Update(CModelRender& modelRender, const EnPlayerState& playerState);

			/**
			 * @brief アニメーションクリップを取得する関数。
			 * @return アニメーションクリップ
			*/
			CAnimationClip* GetAnimationClip() {

				return m_animationClip;
			}

			/**
			 * @brief アニメーションの数を取得する関数。
			 * @return アニメーションの数
			*/
			const int GetAnimationNum()const {

				return enAnim_Num;
			}

		private:
			CAnimationClip m_animationClip[enAnim_Num];		//アニメーションクリップ
			Animation m_animation[enAnimation_BlendNum];	//アニメーション
			Skeleton m_skeleton[enAnimation_BlendNum];		//スケルトン
			float m_animationSpeed = 1.0f;				//アニメーションの再生速度
		};
	}
}

