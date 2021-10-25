#pragma once

namespace nsMyGame {

	namespace nsEnemy {

		class CEnemyAnimation
		{
		protected:
			/**
			 * @brief 初期化関数。
			*/
			virtual void Init() = 0;

			/**
			 * @brief アニメーションを再生する関数。
			*/
			//virtual void PlayAnimation(CModelRender& modelRender, const EnEnemyState& enemyState) = 0;
		public:
			/**
			 * @brief アニメーションクリップを取得する関数。
			 * @return アニメーションクリップ
			*/
			virtual CAnimationClip* GetAnimationClip() {

				return m_animClip;
			}

			/**
			 * @brief アニメーション数を取得する関数。
			 * @return アニメーション数 
			*/
			int GetAnimationNum()const {
				
				return m_animNum;
			}
		public:
			CAnimationClip* m_animClip;
			int m_animNum;
		};
		

	}
}

