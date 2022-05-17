#include "stdafx.h"
#include "LockOnCamera.h"
#include "constCamera.h"

namespace nsMyGame {

	namespace nsCamera {

		namespace {

			constexpr float c_lockOnCameraVelocityMax = 1000.0f;	//移動速度の最大値
			constexpr float c_searchPlayerAngle = 60.0f;			//プレイヤーの視野角
			constexpr float c_searchDistance = 800.0f;				//索敵距離
			constexpr float c_enemyLockOnAddY = 80.0f;				//ロックオンアイコンの場所を決める加算座標
			constexpr float c_inputBorder = 0.5f;					//右スティックの入力判定が起こる値
		}

		bool CLockOnCamera::StartSub() {

			//ばねカメラの初期化。
			m_springCamera.Init(
				*g_camera3D,					//ばねカメラの処理を行うカメラを指定する。
				c_lockOnCameraVelocityMax,		//カメラの移動速度の最大値。
				true,							//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
				c_cameraCollisionRadius			//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
			);

			//遠平面を設定。
			m_springCamera.SetFar(c_cameraFar);

			//カメラを瞬間的に切り替えないように設定。
			m_springCamera.NonRefresh();

			return true;
		}

		void CLockOnCamera::UpdateSub() {

			//ロックオンカメラを更新。
			UpdateLockOnCamera();

			//バネカメラに注視点と視点を設定する。
			m_springCamera.SetPosition(m_position);
			m_springCamera.SetTarget(m_target);

			//ばねカメラを更新。
			m_springCamera.Update();

			//ロックオンアイコンを更新。
			UpdateLockOnMarker();
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

				CVector3 toEnemyVecNormal = toEnemyVec;
				toEnemyVecNormal.Normalize();

				//カメラの視野角に入っているか求める。
				float carentAngle = acosf(toEnemyVecNormal.Dot(playerForward));

				//一定の距離の範囲にいて、かつ死んでいない。
				if (toEnemyVec.Length() < c_searchDistance
					&& !enemy->IsDeath()
					) {

					//プレイヤーから敵に向かって伸びるベクトルを正規化。
					toEnemyVec.Normalize();

					//入ってる。
					if (fabsf(carentAngle) < CMath::PI * (c_searchPlayerAngle / 180.0f)
						&& fabsf(carentAngle) < angle		//かつこれまでの視野角より内側にいる。

						|| angle < 0.0f					//もしくは初めての検索。
						)
					{
						switch (r3Input) {

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
							if (m_lockOnEnemy != enemy && Cross(toEnemyVec, playerForward).y <= 0.0f) {

								//視野角（ラジアン）を更新。
								angle = fabsf(carentAngle);

								//ロックオン対象をこの敵に変更。
								m_lockOnEnemy = enemy;

								//ロックオンできた。
								canLockOn = true;
								return false;
							}
							break;
							//右入力状態なら左側の敵をロックオン対象に切り替える。
						case Input_Left:
							if (m_lockOnEnemy != enemy && Cross(toEnemyVec, playerForward).y >= 0.0f) {

								//視野角（ラジアン）を更新。
								angle = fabsf(carentAngle);

								//ロックオン対象をこの敵に変更。
								m_lockOnEnemy = enemy;

								//ロックオンできた。
								canLockOn = true;
								return false;
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

		void CLockOnCamera::InitCameraToPlayerVec() {

			//ロックオン対象の敵の座標から注視点を求める。
			m_target = m_lockOnEnemy->GetPosition();

			//少し上に設定。
			m_target.y += c_addCameraTarget;

			//プレイヤーを検索。
			nsPlayer::CPlayer* player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//プレイヤーの座標を取得。
			CVector3 playerPosition = player->GetPosition();
			playerPosition.y += c_addCameraTarget;

			//敵からプレイヤーに向かって伸びるベクトルを求める（y成分は削除）。
			CVector3 enemyToPlayerVecYZero = playerPosition - m_target;
			enemyToPlayerVecYZero.y = 0.0f;

			//正規化。
			CVector3 enemyToPlayerVecNormal = enemyToPlayerVecYZero;
			enemyToPlayerVecNormal.Normalize();

			//プレイヤーからカメラに伸びるベクトルのy成分を保存。
			float cameraToPlayerVecY = c_defaultCameraVec.y;

			//カメラベクトルの長さを保存。
			float cameraVecLength = m_cameraToPlayerVec.Length();

			//座標を計算。
			m_cameraToPlayerVec = enemyToPlayerVecNormal * m_cameraToPlayerVec.Length();

			//y成分を定数に変更。
			m_cameraToPlayerVec.y = c_defaultCameraVec.y;

			//正規化。
			m_cameraToPlayerVec.Normalize();

			//保存しておいたベクトルの長さ倍してカメラベクトルの距離を一定にする。
			m_cameraToPlayerVec *= cameraVecLength;
		}

		void CLockOnCamera::UpdateLockOnCamera() {

			//ロックオン対象の敵の座標から注視点を求める。
			m_target = m_lockOnEnemy->GetPosition();

			//少し上に設定。
			m_target.y += c_addCameraTarget;

			//プレイヤーを検索。
			nsPlayer::CPlayer* player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

			//プレイヤーの座標を取得。
			CVector3 playerPosition = player->GetPosition();
			playerPosition.y += c_addCameraTarget;

			//敵からプレイヤーに向かって伸びるベクトルを求める。
			CVector3 enemyToPlayerVec = playerPosition - m_target;
			CVector3 enemytoPlayerVecHalf = enemyToPlayerVec;
			enemytoPlayerVecHalf /= 2.0f;

			playerPosition.y += 30.0f;
			m_target = playerPosition;

			//m_target = m_target + enemytoPlayerVecHalf;

			CVector3 enemyToPlayerVecYZero = enemyToPlayerVec;
			enemyToPlayerVecYZero.y = 0.0f;

			//正規化。
			CVector3 enemyToPlayerVecNormal = enemyToPlayerVecYZero;
			enemyToPlayerVecNormal.Normalize();

			//プレイヤーからカメラに伸びるベクトルのy成分を保存。
			float cameraToPlayerVecY = m_cameraToPlayerVec.y;

			//y成分を消して斜辺の長さをカット。
			m_cameraToPlayerVec.y = 0.0f;

			m_cameraToPlayerVec = enemyToPlayerVecNormal * m_cameraToPlayerVec.Length();
			m_cameraToPlayerVec.y = cameraToPlayerVecY;

			//プレイヤーの少し後ろに視点を設定。
			m_position = m_target + m_cameraToPlayerVec;
			//m_position = m_target + enemyToPlayerVec + m_cameraToPlayerVec;

			//ロックオン対象を更新。
			ChangeLockOnEnemy();

			//もしロックオン対象の敵が死んだら
			//もしくは索敵距離を超えたら
			//カメラを無効化する。
			if (m_lockOnEnemy->IsDeath()
				|| enemyToPlayerVec.Length() > c_searchDistance
				) {

				m_isEnable = false;
			}
		}

		void CLockOnCamera::UpdateLockOnMarker() {

			//ロックオン中の敵の座標を取得。
			CVector3 enemyPosition = m_lockOnEnemy->GetPosition();

			//ロックオン座標を調整。
			enemyPosition.y += 100.0f;

			//enemyPosition = m_target;

			//スクリーン空間上での敵の座標を計算する。
			CVector2 screenPosEnemy = CVector2::Zero;
			m_springCamera.GetCamera()->CalcScreenPositionFromWorldPosition(screenPosEnemy, enemyPosition);

			//ロックオンアイコンを更新。
			m_lockOnMarker.UpdateMarker(screenPosEnemy);
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
}