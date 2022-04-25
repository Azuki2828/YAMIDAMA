#include "stdafx.h"
#include "LockOnCamera.h"

namespace nsMyGame {

	namespace {

		constexpr float c_addCameraPosition = -300.0f;
		constexpr float c_addCameraTarget = 50.0f;
		constexpr float c_addCameraPositionY = 250.0f;
		constexpr float c_searchPlayerAngle = 60.0f;
		constexpr float c_searchDistance = 500.0f;
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
		m_springCamera.SetFar(80000.0f);

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
	}

	const bool CLockOnCamera::LockOnEnemy() {

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

			//一定の距離の範囲にいる。
			if (toEnemyVec.Length() < c_searchDistance) {

				//プレイヤーから敵に向かって伸びるベクトルを正規化。
				toEnemyVec.Normalize();

				//入ってる。
				if (fabsf(carentAngle) < CMath::PI * c_searchPlayerAngle
					&& fabsf(carentAngle) < angle		//かつこれまでの視野角より内側にいる。

					|| angle < 0.0f					//もしくは初めての検索。
					)
				{
					//視野角（ラジアン）を更新。
					angle = fabsf(carentAngle);

					//ロックオン対象をこの敵に変更。
					m_lockOnEnemy = enemy;

					//ロックオンできた。
					canLockOn = true;
				}
			}

			return true;
		});

		return canLockOn;
	}
}