#include "stdafx.h"
#include "PlayerAction.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerAction::Init(const CVector3& position) {

			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				20.0f,			//半径。
				200.0f,			//高さ。
				position		//座標。
			);
		}

		void CPlayerAction::Move(CVector3& position, CVector3& forward, EnPlayerState& playerState) {

			//移動速度を初期化。
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;
			m_rollingSpeed.x = 0.0f;
			m_rollingSpeed.z = 0.0f;

			//カメラの前方方向と右方向を取得。
			CVector3 cameraForward = g_camera3D->GetForward();
			CVector3 cameraRight = g_camera3D->GetRight();

			//XZ平面での前方方向、右方向に変換する。
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			

			//クールタイム中でなく
			if (!IsCoolTime()) {
				//このフレームの移動量を求める。
				//左スティックの入力量を受け取る。
				float lStick_x = g_pad[0]->GetLStickXF();
				float lStick_y = g_pad[0]->GetLStickYF();

				

				//XZ成分の移動速度をクリア。
				m_moveSpeed += cameraForward * lStick_y * 200.0f;	//奥方向への移動速度を加算。
				m_moveSpeed += cameraRight * lStick_x * 200.0f;		//右方向への移動速度を加算。

				//移動していたら移動アニメーションを再生。
				if (lStick_x != 0.0f || lStick_y != 0.0f) {

					//かつBボタンが押されていたら
					if (g_pad[0]->IsPress(enButtonB)) {

						//移動速度を1.5倍に
						m_moveSpeed.x *= 1.5f;
						m_moveSpeed.z *= 1.5f;

						//ダッシュ状態にする。
						playerState = enState_Run;
						m_isDash = true;
					}
					else {
						//歩き状態にする。
						playerState = enState_Walk;
						m_isDash = false;
					}
				}
				//移動入力していない
				else {
					//待機状態にする。
					playerState = enState_Idle;
				}

			}

			if ((playerState == enState_ThreeCombo)) {

				if (c_threeComboCoolTime - m_coolTime < 2.4f) {
					m_moveSpeed += forward * 50.0f;
				}
			}
			////ジャンプ処理。
			//if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら 
			//	//&& m_charaCon.IsOnGround()  //かつ、地面に居たら
			//	) {
			//	//ジャンプする。
			//	m_moveSpeed.y = 400.0f;	//上方向に速度を設定。
			//}

			//ローリング中の移動処理。
			if (playerState == enState_Rolling) {

				float value = c_rollingCoolTime - m_coolTime;

				//指数関数的に速度を求める。
				//ラストの0.2秒間だけは着地のため、移動しない。
				if (c_rollingCoolTime - value > 0.2f) {
					m_rollingSpeed = forward * 150.0f * (-1.5f * pow((value - 1.0f), 2.0f) + 2.0f);
				}

				//ダッシュ中なら移動距離を増やす。
				if (IsDash()) {
					m_rollingSpeed *= 1.5f;
				}
			}

			

			//重力をかける。
			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();


			//ローリングの速度を加算。
			m_moveSpeed += m_rollingSpeed;

			//キャラコンを使用して、座標を更新。
			position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());


			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}
		}

		void CPlayerAction::Rotate(CQuaternion& rotation) {

			//移動ボタンが入力されていなかったら
			if (fabsf(m_moveSpeed.x) < 0.001f
				&& fabsf(m_moveSpeed.z) < 0.001f) {
				//このフレームではキャラは移動していないので旋回する必要はない。
				return;
			}
			//回転角度を求める。
			float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);

			//回転を設定。
			rotation.SetRotationY(-angle);
		}

		void CPlayerAction::Action(EnPlayerState& playerState) {

			//クールタイム中でなく
			if (!IsCoolTime()) {

				//R1ボタンが押されていたら
				if (g_pad[0]->IsTrigger(enButtonRB1)) {

					//弱攻撃状態に。
					playerState = enState_Attack;

					//クールタイムを設定。
					m_coolTime = 1.2f;

				}

				//Aボタンが押されていたら
				if (g_pad[0]->IsTrigger(enButtonA)) {

					//ローリング状態に。
					playerState = enState_Rolling;

					//クールタイムを設定。
					m_coolTime = c_rollingCoolTime;
				}

				//Xボタンが押されていたら
				if (g_pad[0]->IsTrigger(enButtonX)) {

					//3連攻撃状態に。
					playerState = enState_ThreeCombo;

					//クールタイムを設定。
					m_coolTime = c_threeComboCoolTime;
				}
			}
		}

		void CPlayerAction::Update() {

			//クールタイム中なら
			if (IsCoolTime()) {

				//クールタイムを更新。
				m_coolTime -= g_gameTime->GetFrameDeltaTime();
			}
		}
	}
}