#pragma once
#include "stdafx.h"


namespace nsMyGame {

	//優先度。enPriority_Zerothが最も先に描画される。
	enum PriorityNum {
		enPriority_Zeroth,
		enPriority_First,
		enPriority_Second,
		enPriority_Third,
		enPriority_Fourth,
		enPriority_Fifth
	};

	constexpr const char* c_entryPointVSMain = "VSMain";
	constexpr const char* c_entryPointVSSkinMain = "VSSkinMain";
	constexpr const char* c_fxFilePath_Model = "Assets/shader/model.fx";

	constexpr const char* c_classNamePlayer = "player";

	constexpr const char* c_filePathTkmDoor = "Assets/modelData/backGround/door.tkm";	//ドアのtkmファイルパス
	constexpr float c_distanceForOpenDoor = 300.0f;										//ドアを開けるのに必要な距離
	constexpr int c_openDoorRotValue = 90;												//ドアを開けたときの回転角度
	constexpr int c_openDoorRotNum = 1;													//ドアを開けたときに何度ずつ回転させるか


	namespace nsPlayer {

		constexpr const char* c_filePathTkmPlayer = "Assets/modelData/player.tkm";		//プレイヤーのtkmファイルパス
		constexpr const char* c_filePathTksPlayer = "Assets/modelData/player.tks";		//プレイヤーのtksファイルパス
		const CVector3 c_addLightCameraPos = { 0.0f,300.0f,300.0f };					//プレイヤー用のライトカメラに加算する座標
		constexpr const float c_animationInterpolateTime = 0.6f;						//アニメーションの補間時間
		constexpr float c_rollingCoolTime = 1.2f;										//ローリング中のクールタイム
		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム

		const CVector3 c_attackTriggerBoxSize = { 150.0f,150.0f,100.0f };				//プレイヤー攻撃時のトリガーボックスのサイズ
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
			enState_ThreeCombo,
			enState_Guard,

			enState_Num
		};
	}

	namespace nsEnemy {

		constexpr const char* c_filePathTkmEnemy = "Assets/modelData/player.tkm";		//敵のtkmファイルパス
		constexpr const char* c_filePathTksEnemy = "Assets/modelData/player.tks";		//敵のtksファイルパス

		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム
		const CVector3 c_attackTriggerBoxSize = { 150.0f,150.0f,100.0f };				//敵攻撃時のトリガーボックスのサイズ
		constexpr float c_attackTriggerBoxMul = 80.0f;									//敵攻撃時のトリガーボックスの前方向への乗算値
		constexpr float c_attackTriggerBoxAddYPos = 50.0f;								//敵攻撃時のトリガーボックスのY方向への加算値
	}

	namespace nsLight {

		constexpr int c_maxDirectionLightNum = 4;	//ディレクションライトの上限数
		constexpr int c_maxPointLightNum = 100;		//ポイントライトの上限数
	}


	namespace nsFont {

		constexpr int c_maxTextSize = 256;		//テキストの上限数
	}


	namespace nsPostEffect {


		/*------PostEffectComponentBase------*/
		constexpr const char* c_fxFilePathCopySprite = "Assets/shader/Sprite.fx";			//メインレンダリングターゲットにコピーする時に使用するシェーダーのファイルパス
		/*-----------------------------------*/


		/*------GaussianBlur------*/
		constexpr const char* c_fxFilePathGaussianBlur = "Assets/shader/gaussianBlur.fx";	//ガウシアンブラーで使用するシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncXBlur = "VSXBlur";							//横ブラー用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_vsEntryPointFuncYBlur = "VSYBlur";							//縦ブラー用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncBlur = "PSBlur";							//ブラー用のスプライトのピクセルシェーダーのエントリーポイント
		constexpr const int c_gaussianBlurNumWeight = 8;									//ガウシアンブラーの重みの数
		/*------------------------*/


		/*------Bloom------*/
		constexpr const int c_gaussianBlurNum = 4;												//ガウシアンブラーの数
		constexpr const char* c_fxFilePathBloom = "Assets/shader/bloom.fx";						//ブルーム用のシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncBloom = "VSMain";								//ブルーム用の頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncSamplingLuminance = "PSSamplingLuminance";		//輝度抽出用のピクセルシェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncBloomFinal = "PSBloomFinal";					//ブルーム最終のピクセルシェーダーのエントリーポイント
		/*-----------------*/


		/*------FXAA------*/
		constexpr const char* c_fxFilePathFXAA = "Assets/shader/fxaa.fx";	//FXAAで使用するシェーダーのファイルパス
		constexpr const char* c_vsEntryPointFuncFXAA = "VSMain";			//FXAA用のスプライトの頂点シェーダーのエントリーポイント
		constexpr const char* c_psEntryPointFuncFXAA = "PSMain";			//FXAA用のスプライトのピクセルシェーダーのエントリーポイント
		/*----------------*/
	}
}




/**
 * @brief サウンドリスト
*/
enum SoundList {

	enSound_Num
};

/**
 * @brief HUDに登録されているスプライト
*/
enum HUDList {

	enSprite_HUDNum
};

namespace {

	const int c_pad0 = 0;											//0番目のパッド
	const int c_nameSize = 256;										//名前の最大のサイズ
	const int c_textSize = 64;										//テキストの標準サイズ
	const float c_textBitSize = 16.0f;								//テキストのビットサイズ

	const CVector3 c_gravityScale = { 0.0f,-300.0f,0.0f };			//標準の重力

	const CVector3 c_cameraUp = { 0.0f,1.0f,0.0f };					//カメラの標準の上方向

	const CVector2 c_renderTargetW1280H720 = { 1280.0f,720.0f };						//レンダリングターゲットで使用する幅と高さ
	const CVector2 c_shadowMapWH = { 1024.0f,1024.0f };
	const int c_mipLevel1 = 1;														//レンダリングターゲットで使用するミップマップレベル
	const int c_renderArraySize1 = 1;												//レンダリングターゲットで使用するテクスチャ配列のサイズ
	const int c_gaussianBlurNum = 4;												//ガウシアンブラーをかける回数
	const int c_blurPower = 10;														//ブラーの強さ
	const char* c_fxFilePath_DeferredLighting = "Assets/shader/deferredLighting.fx";//スプライトシェーダーのファイルパス
	const char* c_fxFilePath_Sprite = "Assets/shader/sprite.fx";
	const char* c_fxFilePath_ShadowMap = "Assets/shader/sampleDrawShadowMap.fx";

	const int c_initializeTime = 0;
	const int c_restInitialTime = 0;		//スピンロック時の待ちタイムの初期時間
	const int c_spinLockCriteria = 16;		//スピンロック時の基準となる時間

	const CVector4 c_translucentValue_Half = { 1.0f,1.0f,1.0f,0.5f };			//透明率50%
	const CVector4 c_translucentValue_OneThird = { 1.0f,1.0f,1.0f,0.3f };	//透明率70%
	const CVector4 c_translucentValue_Max = { 1.0f,1.0f,1.0f,1.0f };			//透明率0%

	const float c_textShadowThickness = 1.0f;					//テキストの影（枠線）の厚さ

	const CVector3 c_scaleValue_Half = { 0.5f,0.5f,0.5f };		//拡大率50%
	const CVector3 c_scaleValue_ATenth = { 0.1f,0.1f,0.1f };		//拡大率10%
	const CVector3 c_scaleValue_AFifth = { 0.2f,0.2f,0.2f };		//拡大率20%
}

/**
 * @brief 数字の桁数を取得する関数
 * @tparam Num テンプレート
 * @param num 桁数を確かめたい数字
 * @return 桁数
*/
template<typename Num>
int GetDigit(Num num) {

	int digit = 0;
	while (num >= 1) {
		num /= 10;
		digit++;
	}

	return digit;
}