#include "stdafx.h"
#include "LockOnCamera.h"

namespace nsMyGame {

	namespace {

		constexpr float c_cameraFar = 80000.0f;			//カメラの遠平面
		constexpr float c_addCameraPosition = -300.0f;		//カメラの視点を決める値
		constexpr float c_addCameraPositionY = 250.0f;		//カメラの視点（高さ）を決める値
		constexpr float c_addCameraTarget = 50.0f;			//カメラの注視点を決める値
		constexpr float c_searchPlayerAngle = 60.0f;		//プレイヤーの視野角
		constexpr float c_searchDistance = 500.0f;			//索敵距離
		constexpr float c_enemyLockOnAddY = 100.0f;			//ロックオンアイコンの場所を決める加算座標
		constexpr float c_inputBorder = 0.8f;				//右スティックの入力判定が起こる値
	}

	bool CLockOnCamera::StartSub() {

		//ばねカメラの初期化。
		m_springCamera.Init(
			*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
			1000.0f,			//カメラの移動速度の最大値。
			true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
			5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
		);

		//遠平面を設定。
		m_springCamera.SetFar(c_cameraFar);

		return true;
	}

	void CLockOnCamera::UpdateSub() {

		//ロックオン対象の敵の座標から注視点を求める。
		m_target = m_lockOnEnemy->GetPosition();

		//少し上に設定。
		m_target.y += c_addCameraTarget;

		//プレイヤーを検索。
		nsPlayer::CPlayer* player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

		//プレイヤーの座標を取得。
		CVector3 playerPosition = player->GetPosition();

		//プレイヤーの座標から視点を求める。
		m_position = playerPosition;

		//敵に向かって伸びるベクトルを求める。
		CVector3 toEnemyVec = m_target - playerPosition;
		toEnemyVec.y = 0.0f;
		//正規化。
		toEnemyVec.Normalize();

		//プレイヤーの少し後ろに視点を設定。
		m_position += toEnemyVec * c_addCameraPosition;
		m_position.y += c_addCameraPositionY;

		//バネカメラに注視点と視点を設定する。
		m_springCamera.SetPosition(m_position);
		m_springCamera.SetTarget(m_target);

		//ばねカメラを更新。
		m_springCamera.Update();

		//ロックオン対象を更新。
		ChangeLockOnEnemy();

		//もしロックオン対象の敵が死んだらカメラを無効化する。
		if (m_lockOnEnemy->IsDeath()) {

			m_isEnable = false;
		}

		//ロックオン中の敵の座標を取得。
		CVector3 enemyPosition = m_lockOnEnemy->GetPosition();

		//ロックオン座標を調整。
		enemyPosition.y += c_enemyLockOnAddY;

		//スクリーン空間上での敵の座標を計算する。
		CVector2 screenPosEnemy = CVector2::Zero;
		m_springCamera.GetCamera()->CalcScreenPositionFromWorldPosition(screenPosEnemy, enemyPosition);

		//ロックオンアイコンを更新。
		m_lockOnMarker.UpdateMarker(screenPosEnemy);
	}

	const bool CLockOnCamera::LockOnEnemy(R3ButtonInput r3Input) {

		//ロックオンできた？
		bool canLockOn = false;

		//敵の座標を格納する変数。
		CVector3 enemyPosition = CVector3::Zero;

		//視野角の値を格納する変数。
		//初期値は負の値で最初の敵は必ず検索圏に入るように。
		float angle = -1.0f;

		//プレイヤーの前方向と座標を取得。
		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);
		CVector3 playerForward = player->GetForward();
		CVector3 playerPosition = player->GetPosition();
		playerPosition.y = 0.0f;

		//敵を検索。
		QueryGOs<nsEnemy::CEnemy>(c_classNameEnemy, [&](nsEnemy::CEnemy* enemy) {

			//敵の座標を取得。
			enemyPosition = enemy->GetPosition();
			enemyPosition.y = 0.0f;

			//プレイヤーから敵に伸びるベクトルを求める。
			CVector3 toEnemyVec = enemyPosition - playerPosition;

			//プレイヤーの視野角に入っているか求める。
			float carentAngle = acosf(toEnemyVec.Dot(playerForward));

			//一定の距離の範囲にいて、かつ死んでいない。
			if (toEnemyVec.Length() < c_searchDistance
				&& !enemy->IsDeath()
				) {

				//プレイヤーから敵に向かって伸びるベクトルを正規化。
				toEnemyVec.Normalize();

				//入ってる。
				if (fabsf(carentAngle) < CMath::PI * c_searchPlayerAngle
					&& fabsf(carentAngle) < angle		//かつこれまでの視野角より内側にいる。

					|| angle < 0.0f					//もしくは初めての検索。
					)
				{
					switch(r3Input){
					
					//右スティックの入力が無ければ一番近い敵をロックオンするように
					case No_Input:

						//視野角（ラジアン）を更新。
						angle = fabsf(carentAngle);

						//ロックオン対象をこの敵に変更。
						m_lockOnEnemy = enemy;

						//ロックオンできた。
						canLockOn = true;
						break;
					//右入力状態なら右側の敵をロックオン対象に切り替える。
					case Input_Right:
						angle = Cross(toEnemyVec, playerForward).y;
						if (m_lockOnEnemy != enemy && Cross(toEnemyVec, playerForward).y < 0.0f) {

							//視野角（ラジアン）を更新。
							angle = fabsf(carentAngle);

							//ロックオン対象をこの敵に変更。
							m_lockOnEnemy = enemy;

							//ロックオンできた。
							canLockOn = true;
						}
						break;
						//右入力状態なら左側の敵をロックオン対象に切り替える。
					case Input_Left:
						if (m_lockOnEnemy != enemy && Cross(toEnemyVec, playerForward).y > 0.0f) {

							//視野角（ラジアン）を更新。
							angle = fabsf(carentAngle);

							//ロックオン対象をこの敵に変更。
							m_lockOnEnemy = enemy;

							//ロックオンできた。
							canLockOn = true;
						}
						break;
					}
				}
			}

			return true;
		});

		return canLockOn;
	}

	void CLockOnCamera::ChangeLockOnEnemy() {

		//右スティックが入力されている？
		if (CheckR3Input()) {

			//ロックオン対象を更新。
			LockOnEnemy(m_r3Input);
			m_isR3XInput = true;
		}
	}

	const bool CLockOnCamera::CheckR3Input() {

		//現フレームの入力を調べる変数。
		bool isR3XInput = false;

		//入力を調べる。
		float x = g_pad[0]->GetRStickXF();

		//右スティックが左右に倒されているか
		if (x > c_inputBorder) {

			//現フレームで右入力があった。
			isR3XInput = true;
			m_r3Input = Input_Right;
		}
		else if (x < -c_inputBorder) {

			//現フレームで左入力があった。
			isR3XInput = true;
			m_r3Input = Input_Left;
		}
		else {
			//入力がなかったので、前フレームの入力情報を消去。
			m_isR3XInput = false;
			m_r3Input = No_Input;
		}

		//前フレームで入力されていなくて、
		//現フレームで入力がある
		return !m_isR3XInput && isR3XInput;
	}
}