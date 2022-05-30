#pragma once
#include "../../MiniEngine/SpringCamera.h"
#include "CameraBase.h"

namespace nsMyGame {

	//エイリアス宣言
	namespace nsPlayer {

		class CPlayer;
	}

	namespace nsCamera {

		//メインカメラクラス
		class CMainCamera : public CCameraBase
		{
			
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
		private:
			float m_eventTimer = 0.0f;							//イベントタイマー(ボス登場時のカメラワーク)
			SpringCamera m_springCamera;						//ばねカメラ。

			CVector3 m_tar = CVector3::Zero;					//注視点

			//各クラスのポインタ変数
			nsPlayer::CPlayer* m_player = nullptr;
		};
	}
}

