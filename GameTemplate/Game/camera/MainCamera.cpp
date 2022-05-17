#include "stdafx.h"
#include <random>
#include "MainCamera.h"
#include "../player/Player.h"
#include "constCamera.h"

namespace nsMyGame {

	namespace nsCamera {

		namespace {

			constexpr float c_mainCameraVelocityMax = 2500.0f;	//移動速度の最大値
			constexpr float c_mulCameraPosition = 20.0f;			//カメラの座標を決める値
			constexpr float c_mainCameraDampingRate = 0.8f;			//ばねカメラの減衰の仕方
			constexpr float c_mainCameraRotationSpeed = 4.0f;		//カメラの回転速度
			constexpr float c_mainCameraDirectionUp = -0.5f;		//カメラがどれくらい上を向けるか
			constexpr float c_mainCameraDirectionDown = 0.8f;		//カメラがどれくらい下を向けるか
		}

		bool CMainCamera::StartSub() {

			//カメラの加算座標を初期化。
			m_cameraToPlayerVec = c_defaultCameraVec;

			//パラメータを初期化。
			g_camera3D->SetPosition(m_position);
			g_camera3D->SetTarget(m_target);
			g_camera3D->SetFar(c_cameraFar);

			//ばねカメラの初期化。
			m_springCamera.Init(
				*g_camera3D,				//ばねカメラの処理を行うカメラを指定する。
				c_mainCameraVelocityMax,	//カメラの移動速度の最大値。
				true,						//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
				c_cameraCollisionRadius		//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
			);
			m_springCamera.SetDampingRate(c_mainCameraDampingRate);
			return true;
		}

		void CMainCamera::UpdateSub() {

			m_player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);
			//カメラを更新。
			//注視点を計算する。
			m_target = m_player->GetPosition();
			//プレイヤの足元からちょっと上を注視点とする。
			m_target.y += c_addCameraTarget;
			m_target += g_camera3D->GetForward() * c_mulCameraPosition;

			CVector3 toCameraPosOld = m_cameraToPlayerVec;
			//パッドの入力を使ってカメラを回す。
			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();
			//Y軸周りの回転
			CQuaternion qRot;
			qRot.SetRotationDeg(CVector3::AxisY, c_mainCameraRotationSpeed * x);
			qRot.Apply(m_cameraToPlayerVec);
			//X軸周りの回転。
			CVector3 axisX;
			axisX.Cross(CVector3::AxisY, m_cameraToPlayerVec);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, c_mainCameraRotationSpeed * y);
			qRot.Apply(m_cameraToPlayerVec);

			//カメラの回転の上限をチェックする。
			//注視点から視点までのベクトルを正規化する。
			//正規化すると、ベクトルの大きさが１になる。
			//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
			CVector3 toPosDir = m_cameraToPlayerVec;
			toPosDir.Normalize();
			if (toPosDir.y < c_mainCameraDirectionUp) {
				//カメラが上向きすぎ。
				m_cameraToPlayerVec = toCameraPosOld;
			}
			else if (toPosDir.y > c_mainCameraDirectionDown) {
				//カメラが下向きすぎ。
				m_cameraToPlayerVec = toCameraPosOld;
			}


			CVector3 shakeTarget = CVector3::Zero;

			//カメラが揺れ状態ならターゲットを揺らす。
			if (m_state == enShake) {

				std::random_device seed_gen;
				std::mt19937 random(seed_gen());


				shakeTarget = { static_cast<float>(random() % 201) - 100.0f,static_cast<float>(random() % 201) - 100.0f ,static_cast<float>(random() % 201) - 100.0f };
			}

			//視点を計算する。
			m_position = m_target + m_cameraToPlayerVec;

			//バネカメラに注視点と視点を設定する。
			m_springCamera.SetPosition(m_position + shakeTarget);
			m_springCamera.SetTarget(m_target + shakeTarget);
			m_springCamera.Update();
		}
	}
}