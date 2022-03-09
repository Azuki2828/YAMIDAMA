#pragma once

namespace nsMyGame {

	namespace nsPlayer {

		constexpr const char* c_filePathTkmPlayer = "Assets/modelData/human.tkm";		//プレイヤーのtkmファイルパス
		constexpr const char* c_filePathTksPlayer = "Assets/modelData/human.tks";		//プレイヤーのtksファイルパス
		constexpr const float c_animationInterpolateTime = 0.6f;						//アニメーションの補間時間
		constexpr float c_rollingCoolTime = 1.2f;										//ローリング中のクールタイム
		constexpr float c_attackCoolTime = 0.8f;										//通常攻撃中のクールタイム
		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム
		const CVector3 c_playerStartPosition = { 0.0f,0.0f,0.0f };						//開始時のプレイヤーの座標

		constexpr const int c_playerMaxHP = 100;										//プレイヤーの最大HP
		const CVector3 c_attackTriggerBoxSize = { 15.0f, 110.0f, 15.0f };				//プレイヤー攻撃時のトリガーボックスのサイズ
		constexpr float c_attackTriggerBoxMul = 80.0f;									//プレイヤー攻撃時のトリガーボックスの前方向への乗算値
		constexpr float c_attackTriggerBoxAddYPos = 50.0f;								//プレイヤー攻撃時のトリガーボックスのY方向への加算値


		//プレイヤーのステート
		enum EnPlayerState {
			enState_Idle,
			enState_Walk,
			enState_Run,
			enState_Attack,
			enState_AttackBreak,
			enState_Damage,
			enState_Rolling,
			enState_Guard,
			enState_GuardSuccess,
			enState_Death,

			enState_Num
		};
	}
}