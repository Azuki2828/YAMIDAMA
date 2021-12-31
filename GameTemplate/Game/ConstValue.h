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

	/*------クラスの名前------*/
	constexpr const char* c_classNameGameMain = "gameMain";							//ゲームメインクラスの名前
	constexpr const char* c_classNamePlayer = "player";								//プレイヤークラスの名前
	constexpr const char* c_classNameEnemy = "enemy";								//敵クラスの名前
	constexpr const char* c_classNameAppearSprite = "appearSprite";					//ウィンドウクラスの名前
	constexpr const char* c_classNameBackGround = "backGround";						//背景クラスの名前
	constexpr const char* c_classNamePlayerAttackCollision = "playerAttackCollision";	//プレイヤーの攻撃判定の名前
	constexpr const char* c_classnameEnemyAttackCollision = "enemyAttackCollision";		//敵の攻撃判定の名前
	constexpr const char* c_classNameFade = "fade";									//フェードクラスの名前
	constexpr const char* c_classNameGameHUD = "gameHUD";							//HUDクラスの名前
	/*------------------------*/

	//サウンドリスト
	enum EnSoundList {
		enBGM_GameMain,
		enBGM_Boss,
		enSE_Kill,
		enSE_Guard,
		enSE_Rolling,
		enSE_Walk,
		enSE_Death_1,
		enSE_Death_2,
		enSE_GameOver,
		enSE_Select,

		enSound_Num
	};

	/*------サウンドのファイルパス------*/
	constexpr const wchar_t* c_filePathBGM_Boss = L"Assets/sound/BGM_Boss.wav";			//ボス戦BGM
	constexpr const wchar_t* c_filePathBGM_GameMain = L"Assets/sound/BGM_GameMain.wav";	//ゲーム中BGM
	constexpr const wchar_t* c_filePathSE_Kill = L"Assets/sound/SE_Kill.wav";			//斬るSE
	constexpr const wchar_t* c_filePathSE_Guard = L"Assets/sound/SE_Guard.wav";			//ガードSE
	constexpr const wchar_t* c_filePathSE_Rolling = L"Assets/sound/SE_Rolling.wav";		//ローリングSE
	constexpr const wchar_t* c_filePathSE_Walk = L"Assets/sound/SE_Walk.wav";			//歩行SE
	constexpr const wchar_t* c_filePathSE_Death_1 = L"Assets/sound/SE_Death_1.wav";		//倒れるSE
	constexpr const wchar_t* c_filePathSE_Death_2 = L"Assets/sound/SE_Death_2.wav";		//倒れるSE2
	constexpr const wchar_t* c_filePathSE_GameOver = L"Assets/sound/SE_GameOver.wav";	//ゲームオーバーSE
	constexpr const wchar_t* c_filePathSE_Select = L"Assets/sound/SE_Select.wav";		//選択SE
	/*----------------------------------*/

	constexpr const char* c_entryPointVSMain = "VSMain";						//頂点シェーダーのエントリーポイント
	constexpr const char* c_entryPointVSSkinMain = "VSSkinMain";				//スキンありの頂点シェーダーのエントリーポイント
	constexpr const char* c_fxFilePath_Model = "Assets/shader/model.fx";		//G-Buffer作成に使うシェーダーのファイルパス
	constexpr float c_attackCollisionActiveTime = 0.2f;							//攻撃用当たり判定の有効時間

	constexpr const char* c_filePathYouDiedSprite = "Assets/image/youDied2.dds";	//死亡スプライトのファイルパス
	constexpr const char* c_filePathGameClearSprite = "Assets/image/Victory.dds";	//ゲームクリアスプライトのファイルパス
	const CVector2 c_spriteYouDiedWH = { 1740.0f, 180.0f };							//死亡スプライトの幅と高さ
	const CVector3 c_spriteSizeYouDied = { 0.8f,0.8f,0.8f };						//死亡スプライトの拡大率

	constexpr const char* c_filePathFadeSprite = "Assets/image/fade.dds";			//フェード画像のファイルパス
	constexpr const char* c_filePathNewGameSprite = "Assets/image/NEWGAME.dds";		//NEW GAMEテキスト画像のファイルパス
	constexpr const char* c_filePathTextBackSprite = "Assets/image/TextBack.dds";	//テキストの後ろに配置する画像のファイルパス

	constexpr const char* c_filePathTkmDoor = "Assets/modelData/backGround/door.tkm";	//ドアのtkmファイルパス
	constexpr float c_distanceForOpenDoor = 200.0f;										//ドアを開けるのに必要な距離
	constexpr int c_openDoorRotValue = 90;												//ドアを開けたときの回転角度
	constexpr int c_openDoorRotNum = 1;													//ドアを開けたときに何度ずつ回転させるか
	constexpr const char* c_filePathTextSprite = "Assets/image/text.dds";				//テキストスプライトのファイルパス
	const CVector3 c_textSpritePosition = { 0.0f,-250.0f, 0.0f };						//テキストのスプライトの座標
	const CVector3 c_textSpriteSize = { 0.45f, 0.1f, 0.2f };							//テキストのスプライトの拡大率
	const CVector2 c_textSpriteWH = { 1280.0f,720.0f };									//テキストのスプライトの幅と高さ

	const CVector2 c_textPosition = { -50.0f,-240.0f };									//テキストの座標
	const CVector2 c_textPosition_getKey = { -80.0f,-240.0f };							//[Key get.]テキストの座標
	constexpr float c_textSize = 0.5f;													//テキストの拡大率


	constexpr const char16_t* c_filePathFireEffect = u"Assets/effect/fire.efk";			//炎エフェクトのファイルパス
	const CVector3 c_fireEffectScale = { 10.0f,10.0f,10.0f };							//炎エフェクトの拡大率

	constexpr UINT c_frameBufferWidth = 1280;		//フレームバッファの幅
	constexpr UINT c_frameBufferHeight = 720;		//フレームバッファの高さ
	constexpr int c_tileWidth = 16;					//TBRで分割するタイルの横の数
	constexpr int c_tileHeight = 16;				//TBRで分割するタイルの縦の数

	const CVector3 c_firePointLightColor = { 10.0f,5.0f,5.0f };		//炎用のポイントライトのカラー
	constexpr float c_firePointLightRange = 600.0f;					//炎用のポイントライトの影響範囲
	constexpr float c_firePointLightAffectParam = 5.5f;				//炎用のポイントライトの減衰率

	const CVector3 c_addFireEffectPosition = { 0.0f,5.0f,35.0f };	//炎エフェクトの座標調整のために加算するもの

	constexpr const char* c_filePathTitleSprite = "Assets/image/title.dds";	//タイトル画像のファイルパス

	namespace nsPlayer {

		constexpr const char* c_filePathTkmPlayer = "Assets/modelData/player2.tkm";		//プレイヤーのtkmファイルパス
		constexpr const char* c_filePathTksPlayer = "Assets/modelData/player2.tks";		//プレイヤーのtksファイルパス
		constexpr const float c_animationInterpolateTime = 0.6f;						//アニメーションの補間時間
		constexpr float c_rollingCoolTime = 1.2f;										//ローリング中のクールタイム
		constexpr float c_attackCoolTime = 0.8f;										//通常攻撃中のクールタイム
		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム
		const CVector3 c_playerStartPosition = { 0.0f,0.0f,0.0f };					//開始時のプレイヤーの座標

		constexpr const int c_playerMaxHP = 100;									//プレイヤーの最大HP
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
			enState_ThreeCombo,
			enState_Guard,
			enState_GuardSuccess,
			enState_Death,

			enState_Num
		};
	}

	namespace nsEnemy {

		constexpr const char* c_filePathTkmEnemy = "Assets/modelData/player2.tkm";		//敵のtkmファイルパス
		constexpr const char* c_filePathTksEnemy = "Assets/modelData/player2.tks";		//敵のtksファイルパス
		constexpr const char* c_filePathTkmBoss = "Assets/modelData/boss.tkm";			//ボスのtkmファイルパス
		constexpr const char* c_filePathTksBoss = "Assets/modelData/boss.tks";			//ボスのtksファイルパス

		constexpr const int c_bossMaxHP = 100;											//ボスの最大HP
		constexpr float c_threeComboCoolTime = 3.0f;									//3連続攻撃中のクールタイム
		const CVector3 c_attackTriggerBoxSize = { 15.0f, 110.0f, 15.0f };				//敵攻撃時のトリガーボックスのサイズ
		const CVector3 c_bossAttackTriggerBoxSize = { 145.0f, 90.0f, 90.0f };			//ボス攻撃時のトリガーボックスのサイズ
		constexpr float c_attackTriggerBoxMul = 80.0f;									//敵攻撃時のトリガーボックスの前方向への乗算値
		constexpr float c_attackTriggerBoxAddYPos = 50.0f;								//敵攻撃時のトリガーボックスのY方向への加算値
	}

	namespace nsItem {

		constexpr float c_needToDistanceForGetItem = 100.0f;							//アイテムを回収するのに必要な距離
		constexpr const char16_t* c_itemEffectFilePath = u"Assets/effect/item.efk";		//アイテムエフェクトのファイルパス
		const CVector3 c_itemEffectSize = { 10.0f,10.0f,10.0f };						//アイテムエフェクトの拡大率
		constexpr float c_itemEffectReplayTime = 1.0f;									//エフェクトを再生成する間隔


	}

	namespace nsLight {

		constexpr int c_maxDirectionLightNum = 4;	//ディレクションライトの上限数
		constexpr int c_maxPointLightNum = 1000;	//ポイントライトの上限数
		constexpr int c_tileNum = 3600;				//タイルの数。
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
	const CVector2 c_shadowMapWH = { 4096.0f,4096.0f };									//シャドウマップの幅と高さ
	const int c_mipLevel1 = 1;															//レンダリングターゲットで使用するミップマップレベル
	const int c_renderArraySize1 = 1;													//レンダリングターゲットで使用するテクスチャ配列のサイズ
	const int c_gaussianBlurNum = 4;													//ガウシアンブラーをかける回数
	const int c_blurPower = 10;															//ブラーの強さ
	const char* c_fxFilePath_DeferredLighting = "Assets/shader/deferredLighting.fx";	//ディファードライティングに使うシェーダーのファイルパス
	const char* c_fxFilePath_Sprite = "Assets/shader/sprite.fx";						//スプライトシェーダーのファイルパス
	const char* c_fxFilePath_ShadowMap = "Assets/shader/sampleDrawShadowMap.fx";		//シャドウマップ描画に使うシェーダーのファイルパス

	const int c_initializeTime = 0;
	const int c_restInitialTime = 0;		//スピンロック時の待ちタイムの初期時間
	const int c_spinLockCriteria = 16;		//スピンロック時の基準となる時間

	const CVector4 c_translucentValue_Half = { 1.0f,1.0f,1.0f,0.5f };		//透明率50%
	const CVector4 c_translucentValue_OneThird = { 1.0f,1.0f,1.0f,0.3f };	//透明率70%
	const CVector4 c_translucentValue_Zero = { 1.0f,1.0f,1.0f,0.0f };		//透明率100%
	const CVector4 c_translucentValue_Max = { 1.0f,1.0f,1.0f,1.0f };		//透明率0%

	const float c_textShadowThickness = 1.0f;								//テキストの影（枠線）の厚さ

	const CVector3 c_scaleValue_Half = { 0.5f,0.5f,0.5f };					//拡大率50%
	const CVector3 c_scaleValue_ATenth = { 0.1f,0.1f,0.1f };				//拡大率10%
	const CVector3 c_scaleValue_AFifth = { 0.2f,0.2f,0.2f };				//拡大率20%
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