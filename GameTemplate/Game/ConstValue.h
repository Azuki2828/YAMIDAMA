#pragma once
#include "stdafx.h"

//////////////////////////////////////////////////
///　２つ以上のクラスで使う定数をまとめている　///
//////////////////////////////////////////////////



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

	const int PAD_0 = 0;
	const int NAME_SIZE = 256;										//名前の最大のサイズ
	const int TEXT_SIZE = 64;										//テキストの標準サイズ
	const float TEXT_BIT_SIZE = 16.0f;								//テキストのビットサイズ

	const Vector3 GRAVITY_VALUE = { 0.0f,-300.0f,0.0f };			//標準の重力

	const Vector3 CAMERA_UP = { 0.0f,1.0f,0.0f };					//カメラの標準の上方向

	const Vector2 RENDER_TARGET_W1280H720 = { 1280.0f,720.0f };						//レンダリングターゲットで使用する幅と高さ
	const int MIP_LEVEL1 = 1;														//レンダリングターゲットで使用するミップマップレベル
	const int RENDER_ARRAY_SIZE1 = 1;												//レンダリングターゲットで使用するテクスチャ配列のサイズ
	const int GAUSSIAN_BLUR_NUM = 4;												//ガウシアンブラーをかける回数
	const int BLUR_POWER = 10;														//ブラーの強さ
	const char* SPRITE_SHADER_FILE_PATH = "Assets/shader/defferdLighting.fx";		//スプライトシェーダーのファイルパス
	const char* SPRITE_SHADER_MONOCHROME_FILE_PATH = "Assets/shader/sample2D.fx";	//モノクロ表示用のシェーダーのファイルパス
	const char* HUD_FILE_PATH = "Assets/shader/sprite.fx";

	const int INITIALIZE_TIME = 0;
	const int REST_INITIAL_TIME = 0;		//スピンロック時の待ちタイムの初期時間
	const int SPIN_LOCK_CRITERIA = 16;		//スピンロック時の基準となる時間

	const Vector4 TRANSLUCENT_VALUE_HALF = { 1.0f,1.0f,1.0f,0.5f };			//透明率50%
	const Vector4 TRANSLUCENT_VALUE_ONE_THIRD = { 1.0f,1.0f,1.0f,0.3f };	//透明率70%
	const Vector4 TRANSLUCENT_VALUE_MAX = { 1.0f,1.0f,1.0f,1.0f };			//透明率0%

	const float TEXT_SHADOW_THICKNESS = 1.0f;					//テキストの影（枠線）の厚さ

	const Vector3 SCALE_VALUE_HALF = { 0.5f,0.5f,0.5f };		//拡大率50%
	const Vector3 SCALE_VALUE_A_TENTH = { 0.1f,0.1f,0.1f };		//拡大率10%
	const Vector3 SCALE_VALUE_A_FIFTH = { 0.2f,0.2f,0.2f };		//拡大率20%
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