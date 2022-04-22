#include "stdafx.h"
#include "LockOnCamera.h"

namespace nsMyGame {

	namespace {

		constexpr float c_addCameraPosition = -300.0f;
		constexpr float c_addCameraTarget = 50.0f;
		constexpr float c_addCameraPositionY = 250.0f;
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

		//敵を検索。
		LockOnEnemy();

		return true;
	}

	void CLockOnCamera::UpdateSub() {

		//カメラが切り替わったら敵を再検索。
		if (m_switched) { LockOnEnemy(); }

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

	void CLockOnCamera::LockOnEnemy() {

		//敵との距離を取得する変数。
		float enemyPosition = 0.0f;

		//敵を検索。
		QueryGOs<nsEnemy::CEnemy>(c_classNameEnemy, [&](nsEnemy::CEnemy* enemy) {

			//初めて検索した敵なら、そのまま代入。
			//２人目以降の敵なら、これまでの敵との距離と比較してより短いほうの敵を取得。
			if (enemyPosition == 0.0f
				|| enemyPosition > enemy->GetPosition().Length()
			)
			{
				//距離のサンプルを更新。
				enemyPosition = enemy->GetPosition().Length();

				//ロックオン対象の敵を更新。
				m_lockOnEnemy = enemy;
			}

			return true;
			});
	}
}