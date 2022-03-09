#pragma once
#include "constPlayer.h"

namespace nsMyGame {

	namespace nsPlayer {

		//アニメーションのリスト
		enum EnAnimationList {
			enAnim_Idle,
			enAnim_Walk,
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

		//プレイヤーのアニメーションを扱うクラス
		class CPlayerAnimation
		{
		public:
			/**
			 * @brief 初期化関数。
			*/
			void Init();

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
		};
	}
}

