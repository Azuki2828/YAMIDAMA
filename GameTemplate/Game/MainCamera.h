#pragma once
#include "../../MiniEngine/SpringCamera.h"

namespace nsMyGame {

	//エイリアス宣言
	namespace nsPlayer {

		class CPlayer;
	}

	//メインカメラクラス
	class CMainCamera : public CIGameObject
	{
	public:
		/**
		 * @brief Update()関数の前に一度だけ呼ばれる関数。
		 * @return 成功した？
		*/
		bool Start()override final;

		/**
		 * @brief 更新関数。
		*/
		void Update()override final;
	private:
		SpringCamera m_springCamera;						//ばねカメラ。
		CVector3 m_pos = CVector3::Zero;					//視点
		CQuaternion m_rot = CQuaternion::Identity;			//回転
		CVector3 m_sca = CVector3::One;						//拡大

		CVector3 m_tar = CVector3::Zero;					//注視点
		CVector3 m_toCameraPos = { 0.0f,150.0f,-200.0f };	//カメラの追加座標

		//各クラスのポインタ変数
		nsPlayer::CPlayer* m_player = nullptr;
	};
}

