#include "stdafx.h"
#include "PlayerAction.h"
#include "Player.h"
#include "../AttackCollision.h"

namespace nsMyGame {

	namespace nsPlayer {

		void CPlayerAction::Init(const CVector3& position, const CQuaternion& rotation, const CVector3& forward, Bone* swordBone) {

			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				20.0f,			//半径。
				100.0f,			//高さ。
				position		//座標。
			);

			m_swordBone = swordBone;

			//剣のボーンのワールド行列を取得。
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
			if (!IsCoolTime() && !IsGuard() && !IsGuardSccessCoolTime()) {
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

			//通常攻撃中なら
			if ((playerState == enState_Attack)) {
			
				float value = c_attackCoolTime - m_coolTime;
				if (c_attackCoolTime - m_coolTime > 0.1f && c_attackCoolTime - m_coolTime < 0.4f) {
					m_rollingSpeed = forward * 20.0f *(-4.0f * pow((value - 1.0f), 2.0f) + 8.0f);
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
					m_rollingSpeed = forward * 400.0f;// *(-1.5f * pow((value - 1.0f), 2.0f) + 2.0f);
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

			//キャラコンを使用して、座標を更新。
			position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());


			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}
		}

		void CPlayerAction::Rotate(CQuaternion& rotation) {

			//入力量を調べる。
			float lStick_x = g_pad[0]->GetLStickXF();
			float lStick_y = g_pad[0]->GetLStickYF();

			//移動ボタンが入力されていなかったら
			if (fabsf(lStick_x) < 0.001f
				&& fabsf(lStick_y) < 0.001f) {
				//このフレームではキャラは移動していないので旋回する必要はない。
				return;
			}

			//カメラの前方方向と右方向を取得。
			CVector3 cameraForward = g_camera3D->GetForward();
			CVector3 cameraRight = g_camera3D->GetRight();

			//XZ平面での前方方向、右方向に変換する。
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//XZ成分の移動速度をクリア。
			CVector3 rotSource = CVector3::Zero;
			rotSource += cameraForward * lStick_y;		//奥方向を計算。
			rotSource += cameraRight * lStick_x;		//右方向を計算。

			//回転角度を求める。
			float angle = atan2(-rotSource.x, rotSource.z);

			//回転を設定。
			rotation.SetRotationY(-angle);
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

		void CPlayerAction::CreateAttackCollision() {

			auto player = FindGO<CPlayer>(c_classNamePlayer);
			
			//剣のボーンのワールド行列を取得する。
			CMatrix swordBaseMatrix = player->GetModelRender()->GetSkeleton()->GetBone(66)->GetWorldMatrix();

			//コリジョンオブジェクトを作成する。
			auto collisionObject = NewGO<CAttackCollision>(enPriority_Zeroth, c_classNamePlayerAttackCollision);

			//有効時間を設定。
			collisionObject->SetActiveTime(0.2f);

			//ボックス状のコリジョンを作成する。
			collisionObject->CreateBox(player->GetPosition(), CQuaternion::Identity, c_attackTriggerBoxSize);

			//剣のボーンのワールド行列をコリジョンに適用させる。
			collisionObject->SetWorldMatrix(swordBaseMatrix);
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