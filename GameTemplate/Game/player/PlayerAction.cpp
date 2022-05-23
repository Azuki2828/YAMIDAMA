#include "stdafx.h"
#include "PlayerAction.h"
#include "Player.h"
#include "../AttackCollision.h"
#include "../camera/CameraManager.h"

namespace nsMyGame {

	namespace nsPlayer {
		void CPlayerAction::Init(const CVector3& position, const CQuaternion& rotation, const CVector3& forward, Bone* swordBone) {

			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				20.0f,			//半径
				100.0f,			//高さ
				position		//座標
			);
			m_swordBone = swordBone;
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定のインスタンスを初期化。
			m_attackCollision.Create(m_position, m_rotation);

			//当たり判定の座標と回転を更新。
			m_attackCollision.UpdatePositionAndRotation(swordBaseMatrix);

			//当たり判定をしないように設定。
			m_attackCollision.Deactivate();
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

			//クールタイム中でなく、ガード中、ガード成功時でなかったら
			if (!IsCoolTime() && !IsGuardSccessCoolTime()) {
				//このフレームの移動量を求める。
				//左スティックの入力量を受け取る。
				float lStick_x = g_pad[0]->GetLStickXF();
				float lStick_y = g_pad[0]->GetLStickYF();

				//XZ成分の移動速度をクリア。
				m_moveSpeed += cameraForward * lStick_y * 200.0f;	//奥方向への移動速度を加算。
				m_moveSpeed += cameraRight * lStick_x * 200.0f;		//右方向への移動速度を加算。

				//カメラタイプを取得。
				auto cameraManager = FindGO<nsCamera::CCameraManager>(c_classNameCameraManager);
				auto cameraState = cameraManager->GetCameraType();

				//移動していたら移動アニメーションを再生。
				if (lStick_x != 0.0f || lStick_y != 0.0f) {

					//入力によって移動している。
					m_isMove = true;

					//ロックオン状態なら横移動アニメーションに。
					if (cameraState == nsCamera::enCamera_LockOn) {

						if (lStick_x > 0.0f) {
							playerState = enState_RightWalk;
						}
						else {
							playerState = enState_LeftWalk;
						}
					}
					//Bボタンが押されていたら
					else if (g_pad[0]->IsPress(enButtonB)) {

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
					m_isMove = false;
				}
			}

			//通常攻撃中なら
			if ((playerState == enState_Attack)) {
			
				float value = c_attackCoolTime - m_coolTime;
				if (c_attackCoolTime - m_coolTime > 0.1f && c_attackCoolTime - m_coolTime < 0.4f) {
					m_moveSpeed = forward * 20.0f *(-4.0f * pow((value - 1.0f), 2.0f) + 8.0f);
				}
			}

			//ガード成功時のノックバック
			if (IsGuardSccessCoolTime()) {

				m_moveSpeed -= forward * 150.0f;
			}

			//ローリング中の移動処理。
			if (playerState == enState_Rolling) {

				float value = c_rollingCoolTime - m_coolTime;

				//指数関数的に速度を求める。
				//ラストの0.2秒間だけは着地のため、移動しない。
				if (c_rollingCoolTime - value > 0.8f) {
					m_rollingSpeed = forward * 400.0f;
				}
				else if (c_rollingCoolTime - value > 0.2f) {
					m_rollingSpeed = forward * 100.0f;
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

			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}

			//キャラコンを使用して、座標を更新。
			position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		}

		void CPlayerAction::Rotate(CQuaternion& rotation, const CVector3& forward, const EnPlayerState& playerState) {

			//特定状態なら終了。
			if (playerState == enState_Rolling
				|| playerState == enState_Damage
				|| playerState == enState_Attack
				|| playerState == enState_AttackBreak
				) { return; }

			//XZ成分の移動速度をクリア。
			CVector3 rotSource = CVector3::Zero;

			//カメラ情報を取得。
			auto cameraManager = FindGO<nsCamera::CCameraManager>(c_classNameCameraManager);

			//ロックオン中なら
			if (cameraManager->GetCameraType() == nsCamera::enCamera_LockOn) {

				//プレイヤーを検索。
				auto player = FindGO<CPlayer>(c_classNamePlayer);

				//ロックオン中の敵の座標を取得。
				CVector3 lockOnEnemyPosition = cameraManager->GetLockOnEnemyPosition();
				CVector3 vecToEnemy = lockOnEnemyPosition - player->GetPosition();
				//正規化。
				vecToEnemy.Normalize();

				//角度を格納する変数を初期化。
				float angle = 0.0f;

				//ワールドのZ軸と敵方向へのベクトルのなす角（ラジアン）を求める。
				angle = acos(Dot(CVector3::AxisZ, vecToEnemy) / CVector3::AxisZ.Length() * vecToEnemy.Length());

				//二つのベクトルの外積が負の値なら符号を入れかえる。
				if (Cross(CVector3::AxisZ, vecToEnemy).y < 0.0f) {

					angle *= -1.0f;
				}

				//回転を設定。
				rotation.SetRotationY(angle);

				//ここで終了。
				return;
			}

			//入力量を調べる。
			float lStick_x = g_pad[0]->GetLStickXF();
			float lStick_y = g_pad[0]->GetLStickYF();

			//カメラの前方方向と右方向を取得。
			CVector3 cameraForward = g_camera3D->GetForward();
			CVector3 cameraRight = g_camera3D->GetRight();

			//XZ平面での前方方向、右方向に変換する。
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//移動ボタンが入力されていなかったら
			if (fabsf(lStick_x) < 0.001f
				&& fabsf(lStick_y) < 0.001f) {
				//このフレームではキャラは移動していないので入力による旋回はする必要はない。
				return;
			}

			rotSource += cameraForward * lStick_y;		//奥方向を計算。
			rotSource += cameraRight * lStick_x;		//右方向を計算。
			//正規化。
			rotSource.Normalize();

			//前方向と入力方向の内積から回転する必要があるかどうかを調べる。
			if (Dot(forward, rotSource) < 0.99f) {

				//回転速度を求める。
				float rotSpeed = c_rotSpeed * g_gameTime->GetFrameDeltaTime();

				//前方向と入力方向の外積から回転方向を求める。
				if (Cross(forward, rotSource).y >= 0.0f) {

					//時計回り。
					rotation.AddRotationY(rotSpeed);
				}
				else {

					//反時計回り。
					rotation.AddRotationY(-rotSpeed);
				}
			}	
		}

		void CPlayerAction::Action(EnPlayerState& playerState, const bool selectFlag) {

			//クールタイム中でなく
			if (!IsCoolTime()) {

				//R1ボタンが押されたら
				if (g_pad[0]->IsTrigger(enButtonRB1)) {

					//弱攻撃状態に。
					playerState = enState_Attack;

					//クールタイムを設定。
					m_coolTime = c_attackCoolTime;
				}

				//L1ボタンが押されていたら
				if (g_pad[0]->IsPress(enButtonLB1) && !IsGuardSccessCoolTime()) {

					//ガード状態に。
					playerState = enState_Guard;
					m_isGuard = true;
				}
				else {

					//ガード状態を解除。
					m_isGuard = false;
				}

				//何も選んでいない状態でAボタンが押されたら
				if (g_pad[0]->IsTrigger(enButtonA) && !selectFlag) {

					//ローリング状態に。
					playerState = enState_Rolling;

					//クールタイムを設定。
					m_coolTime = c_rollingCoolTime;
				}
			}
		}

		void CPlayerAction::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
		{
			//キーの名前が「attack」の時。
			if (wcscmp(eventName, L"attack") == 0)
			{
				//攻撃中にする。
				m_attackCollision.Activate();
			}
			//キーの名前が「attack_end」の時。
			else if (wcscmp(eventName, L"attackEnd") == 0)
			{
				//攻撃を終わる。
				m_attackCollision.Deactivate();
			}
			else if (wcscmp(eventName, L"startRolling") == 0)
			{
				//ローリングSEを再生。
				CSoundManager::GetInstance()->Play(enSE_Rolling);
			}
			else if (wcscmp(eventName, L"walk") == 0
				|| wcscmp(eventName, L"run") == 0
			)
			{
				//歩行SEを再生。
				CSoundManager::GetInstance()->Play(enSE_Walk);
			}
			else if (wcscmp(eventName, L"death_1") == 0)
			{
				//倒れるSEを再生。
				CSoundManager::GetInstance()->Play(enSE_Death_1);
			}
			else if (wcscmp(eventName, L"death_2") == 0)
			{
				//倒れるSE2を再生。
				CSoundManager::GetInstance()->Play(enSE_Death_2);
			}
		}

		void CPlayerAction::Update() {

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定の座標と回転を更新。
			m_attackCollision.UpdatePositionAndRotation(swordBaseMatrix);

			//当たり判定を更新。
			m_attackCollision.Update();

			//クールタイム中なら
			if (IsCoolTime()) {

				//クールタイムを更新。
				m_coolTime -= g_gameTime->GetFrameDeltaTime();
			}

			//ガード中のクールタイム中なら
			if (IsGuardSccessCoolTime()) {

				//ガード中のクールタイムを更新。
				m_guardSccessCoolTime -= g_gameTime->GetFrameDeltaTime();
			}
		}
	}
}