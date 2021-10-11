#pragma once
#include "stdafx.h"


namespace nsMyGame {

	const Vector2 c_deferredSpriteWH = { 1280.0f,720.0f };

	namespace nsLight {

		constexpr int c_maxDirectionLightNum = 4;	//ディレクションライトの上限数
		constexpr int c_maxPointLightNum = 100;		//ポイントライトの上限数
	}

	namespace nsFont {

		constexpr int c_maxTextSize = 256;
	}
}
/**
 * @brief 優先度。enPriority_Zerothが最も先に描画される。
*/
enum PriorityNum{
	enPriority_Zeroth,
	enPriority_First,
	enPriority_Second,
	enPriority_Third,
	enPriority_Fourth,
	enPriority_Fifth
};

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

	const Vector3 c_gravityScale = { 0.0f,-300.0f,0.0f };			//標準の重力

	const Vector3 c_cameraUp = { 0.0f,1.0f,0.0f };					//カメラの標準の上方向

	const Vector2 c_renderTargetW1280H720 = { 1280.0f,720.0f };						//レンダリングターゲットで使用する幅と高さ
	const Vector2 c_shadowMapWH = { 1024.0f,1024.0f };
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

	const Vector4 c_translucentValue_Half = { 1.0f,1.0f,1.0f,0.5f };			//透明率50%
	const Vector4 c_translucentValue_OneThird = { 1.0f,1.0f,1.0f,0.3f };	//透明率70%
	const Vector4 c_translucentValue_Max = { 1.0f,1.0f,1.0f,1.0f };			//透明率0%

	const float c_textShadowThickness = 1.0f;					//テキストの影（枠線）の厚さ

	const Vector3 c_scaleValue_Half = { 0.5f,0.5f,0.5f };		//拡大率50%
	const Vector3 c_scaleValue_ATenth = { 0.1f,0.1f,0.1f };		//拡大率10%
	const Vector3 c_scaleValue_AFifth = { 0.2f,0.2f,0.2f };		//拡大率20%
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