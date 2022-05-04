#pragma once
#include "../../MiniEngine/SpringCamera.h"
#include "CameraBase.h"

namespace nsMyGame {

	//エイリアス宣言
	namespace nsPlayer {

		class CPlayer;
	}

	//メインカメラクラス
	class CMainCamera : public CCameraBase
	{
		//ステート
		enum EnState {
			enNormal,		//通常
			enShake			//揺れ状態
		};
	private:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool StartSub()override final;

		/**
		 * @brief 更新関数。
		*/
		void UpdateSub()override final;

		void SubSwitched()override final {

		}
	public:
		/**
		 * @brief カメラを揺れ状態にする関数。
		*/
		void ShakeCamera() {

			m_state = enShake;
		}

		/**
		 * @brief カメラの状態を通常状態にする関数。
		*/
		void SetNormalCamera() {

			m_state = enNormal;
		}
	private:
		EnState m_state = enNormal;							//ステート
		float m_eventTimer = 0.0f;							//イベントタイマー(ボス登場時のカメラワーク)
		SpringCamera m_springCamera;						//ばねカメラ。

		CVector3 m_tar = CVector3::Zero;					//注視点
		CVector3 m_toCameraPos = { 0.0f,150.0f,200.0f };	//カメラの追加座標

		//各クラスのポインタ変数
		nsPlayer::CPlayer* m_player = nullptr;
	};
}

