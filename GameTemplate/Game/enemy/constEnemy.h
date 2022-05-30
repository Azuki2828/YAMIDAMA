#pragma once

namespace nsMyGame {

	namespace nsEnemy {

		constexpr const char* c_filePathTkmEnemy = "Assets/modelData/human.tkm";		//敵のtkmファイルパス
		constexpr const char* c_filePathTksEnemy = "Assets/modelData/human.tks";		//敵のtksファイルパス
		constexpr const char* c_filePathTkmBoss = "Assets/modelData/boss.tkm";			//ボスのtkmファイルパス
		constexpr const char* c_filePathTksBoss = "Assets/modelData/boss.tks";			//ボスのtksファイルパス

		constexpr const float c_bossAnimBlendValue = 0.4f;								//ボスのアニメーションの補間時間

		constexpr const int c_bossMaxHP = 100;											//ボスの最大HP
		constexpr const int c_bossAttack = 10;											//ボスの攻撃力
		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム
		const CVector3 c_attackTriggerBoxSize = { 15.0f, 110.0f, 15.0f };				//敵攻撃時のトリガーボックスのサイズ
		const CVector3 c_bossAttackTriggerBoxSize = { 145.0f, 90.0f, 90.0f };			//ボス攻撃時のトリガーボックスのサイズ
		constexpr float c_attackTriggerBoxMul = 80.0f;									//敵攻撃時のトリガーボックスの前方向への乗算値
		constexpr float c_attackTriggerBoxAddYPos = 50.0f;								//敵攻撃時のトリガーボックスのY方向への加算値
	}
}