#pragma once
#include "constPlayer.h"

namespace nsMyGame {

	namespace nsPlayer {

		//アニメーションのリスト
		enum EnAnimationList {
			enAnim_Idle,			//待機
			enAnim_Walk,			//歩行
			enAnim_LeftWalk,		//左回り歩行
			enAnim_RightWalk,		//右回り歩行
			enAnim_Run,				//走り
			enAnim_Attack,			//攻撃
			enAnim_AttackBreak,		//攻撃はじかれた
			enAnim_Damage,			//被弾
			enAnim_Rolling,			//ローリング
			enAnim_Guard,			//ガード
			enAnim_GuardSuccess,	//ガード成功
			enAnim_Death,			//死亡

			enAnim_Num

		};

		//上半身？下半身？
		enum EnBone {

			enUpperBody,	//上半身
			enLowerBody,	//下半身

			enBodyNum
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
			//CAnimationClip* GetAnimationClip() {
			//
			//	return m_animationClip;
			//}

			/**
			 * @brief アニメーションの数を取得する関数。
			 * @return アニメーションの数
			*/
			const int GetAnimationNum()const {

				return enAnim_Num;
			}

			/**
			 * @brief このフレームは移動していると判定。
			*/
			void Move() {

				m_isMove = true;
			}

		private:
			/**
			 * @brief アニメーションクリップを初期化する関数。
			*/
			void InitAnimationClip();

		private:
			//上半身、下半身のボーンの数
			enum {
				c_upperBoneNum = 54,
				c_lowerBoneNum = 11
			};
			bool m_isMove = false;							//動いているかどうか
			CAnimationClip m_animationClip[enBodyNum][enAnim_Num];		//アニメーションクリップ
			Animation m_animation[enBodyNum];				//アニメーション
			Skeleton m_skeleton[enBodyNum];					//スケルトン
			float m_animationSpeed = 2.0f;					//アニメーションの再生速度

			//上半身のボーンの名前のリスト
			const char* m_upperBoneName[c_upperBoneNum] = {
				
				"mixamorig5:Spine",
				"mixamorig5:Spine1",
				"mixamorig5:Spine2",
				"mixamorig5:LeftShoulder",
				"mixamorig5:LeftArm",
				"mixamorig5:LeftForeArm",
				"mixamorig5:LeftHand",
				"mixamorig5:LeftHandIndex1",
				"mixamorig5:LeftHandIndex2",
				"mixamorig5:LeftHandIndex3",
				"mixamorig5:LeftHandIndex4",
				"mixamorig5:LeftHandMiddle1",
				"mixamorig5:LeftHandMiddle2",
				"mixamorig5:LeftHandMiddle3",
				"mixamorig5:LeftHandMiddle4",
				"mixamorig5:LeftHandPinky1",
				"mixamorig5:LeftHandPinky2",
				"mixamorig5:LeftHandPinky3",
				"mixamorig5:LeftHandPinky4",
				"mixamorig5:LeftHandRing1",
				"mixamorig5:LeftHandRing2",
				"mixamorig5:LeftHandRing3",
				"mixamorig5:LeftHandRing4",
				"mixamorig5:LeftHandThumb1",
				"mixamorig5:LeftHandThumb2",
				"mixamorig5:LeftHandThumb3",
				"mixamorig5:LeftHandThumb4",
				"mixamorig5:Neck",
				"mixamorig5:Head",
				"mixamorig5:HeadTop_End",
				"mixamorig5:RightShoulder",
				"mixamorig5:RightArm",
				"mixamorig5:RightForeArm",
				"mixamorig5:RightHand",
				"mixamorig5:RightHandIndex1",
				"mixamorig5:RightHandIndex2",
				"mixamorig5:RightHandIndex3",
				"mixamorig5:RightHandIndex4",
				"mixamorig5:RightHandMiddle1",
				"mixamorig5:RightHandMiddle2",
				"mixamorig5:RightHandMiddle3",
				"mixamorig5:RightHandMiddle4",
				"mixamorig5:RightHandPinky1",
				"mixamorig5:RightHandPinky2",
				"mixamorig5:RightHandPinky3",
				"mixamorig5:RightHandPinky4",
				"mixamorig5:RightHandRing1",
				"mixamorig5:RightHandRing2",
				"mixamorig5:RightHandRing3",
				"mixamorig5:RightHandRing4",
				"mixamorig5:RightHandThumb1",
				"mixamorig5:RightHandThumb2",
				"mixamorig5:RightHandThumb3",
				"mixamorig5:RightHandThumb4"
			};
			//下半身のボーンの名前のリスト
			const char* m_lowerBoneName[c_lowerBoneNum] = {
				
				"mixamorig5:Hips",
				"mixamorig5:LeftUpLeg",
				"mixamorig5:LeftLeg",
				"mixamorig5:LeftFoot",
				"mixamorig5:LeftToeBase",
				"mixamorig5:LeftToe_End",
				"mixamorig5:RightUpLeg",
				"mixamorig5:RightLeg",
				"mixamorig5:RightFoot",
				"mixamorig5:RightToeBase",
				"mixamorig5:RightToe_End"
			};
		};
	}
}

