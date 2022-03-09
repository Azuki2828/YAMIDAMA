#include "stdafx.h"
#include <random>
#include "MainCamera.h"
#include "player/Player.h"

namespace nsMyGame {

	bool CMainCamera::Start() {

		g_camera3D->SetPosition(m_pos);
		g_camera3D->SetTarget(m_tar);
		g_camera3D->SetFar(80000.0f);

		//ばねカメラの初期化。
		m_springCamera.Init(
			*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
			1000.0f,			//カメラの移動速度の最大値。
			true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
			5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
		);

		return true;
	}

	void CMainCamera::Update() {

		CVector3 pos;
		CVector3 target;

		m_player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);
		//カメラを更新。
		//注視点を計算する。
		target = m_player->GetPosition();
		//プレイヤの足元からちょっと上を注視点とする。
		target.y += 80.0f;
		target += g_camera3D->GetForward() * 20.0f;

		CVector3 toCameraPosOld = m_toCameraPos;
		//パッドの入力を使ってカメラを回す。
		float x = g_pad[0]->GetRStickXF();
		float y = g_pad[0]->GetRStickYF();
		//Y軸周りの回転
		CQuaternion qRot;
		qRot.SetRotationDeg(CVector3::AxisY, 2.0f * x);
		qRot.Apply(m_toCameraPos);
		//X軸周りの回転。
		CVector3 axisX;
		axisX.Cross(CVector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, 2.0f * y);
		qRot.Apply(m_toCameraPos);

		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		CVector3 toPosDir = m_toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//カメラが上向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.8f) {
			//カメラが下向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}


		CVector3 shakeTarget = CVector3::Zero;

		//カメラが揺れ状態ならターゲットを揺らす。
		if (m_state == enShake) {

			std::random_device seed_gen;
			std::mt19937 random(seed_gen());

			
			shakeTarget = { static_cast<float>(random() % 201) - 100.0f,static_cast<float>(random() % 201) - 100.0f ,static_cast<float>(random() % 201) - 100.0f};
			int a = 0;
		}

		//視点を計算する。
		pos = target + m_toCameraPos;

		//バネカメラに注視点と視点を設定する。
		m_springCamera.SetFar(80000.0f);
		m_springCamera.SetPosition(pos + shakeTarget);
		m_springCamera.SetTarget(target + shakeTarget);
		m_springCamera.Update();
	}
}