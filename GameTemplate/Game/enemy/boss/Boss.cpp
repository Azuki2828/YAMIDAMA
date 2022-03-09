#include "stdafx.h"
#include "Boss.h"
#include "../../AttackCollision.h"
#include "../../BackGround.h"
#include "../../MainCamera.h"

namespace nsMyGame {

	namespace nsEnemy {

		extern CEnemy* g_pCurrentEnemy;

		bool CBoss::StartSub() {

			//IGameObjectに追加。
			m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

			//tkmファイルとtksファイルを設定。
			m_modelRender->SetFilePathTkm(c_filePathTkmBoss);
			m_modelRender->SetFilePathTks(c_filePathTksBoss);

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//アニメーションクリップを初期化。
			InitAnimationClip();

			//アニメーションを初期化。
			m_modelRender->InitAnimation(m_animationClip, enAnim_Num);

			//影を落とす。
			m_modelRender->SetShadowCasterFlag(true);

			//影を受ける。
			m_modelRender->SetShadowReceiverFlag(true);

			//ステータスを初期化。
			InitStatus();

			//初期化。
			m_modelRender->Init();

			//アニメーションイベント用の関数を設定する。
			m_modelRender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {

				OnAnimationEvent(clipName, eventName);
			});

			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				100.0f,			//半径。
				100.0f,			//高さ。
				m_position		//座標。
			);

			//剣に取り付けられたボーンの番号を読み込む。
			auto swordBoneNum = m_modelRender->GetSkeleton()->FindBoneID(L"RightHand");

			//剣のボーンを取得。
			m_swordBone = m_modelRender->GetSkeleton()->GetBone(swordBoneNum);

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定のインスタンスを初期化。
			m_triggerBox.Create(m_position, m_rotation);

			//当たり判定の座標と回転を更新。
			m_triggerBox.UpdatePositionAndRotation(swordBaseMatrix);

			//当たり判定をしないように設定。
			m_triggerBox.DeactivateAttack();
			m_triggerBox.DeactivateRangeAttack();

			//プレイヤーに気づくためのトリガーボックスを設定。
			m_noticePlayerTriggerBox.CreateBox(m_position, CQuaternion::Identity, { 1500.0f,1000.0f,1500.0f });

			//登場時のクールタイムは2.0秒。
			m_coolTime = 2.0f;

			//ゲーム音楽をボス戦BGMに変更。
			CSoundManager::GetInstance()->Release(enBGM_GameMain);
			CSoundManager::GetInstance()->Play(enBGM_Boss);

			return true;
		}

		void CBoss::UpdateSub() {

			// 現在更新処理を実行中のエネミーのアドレスを代入
			g_pCurrentEnemy = this;

			FindPlayer();

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定のワールド行列を更新。
			m_triggerBox.UpdatePositionAndRotation(swordBaseMatrix);

			//ステートに応じて読み込むPythonスクリプトを変える。
			switch (m_state) {
			case enState_Idle:
				ImportModule("BossIdle");
				break;
			case enState_Walk:
				ImportModule("BossMove");
				break;
			case enState_JumpAttack:
				ImportModule("BossJumpAttack");
				break;
			case enState_SwipingAttack:
				ImportModule("BossSwipingAttack");
				break;
			case enState_Damage:
				ImportModule("EnemyDamage");
				break;
			case enState_Death:
				ImportModule("BossDeath");
				break;
			case enState_Scream:
				ImportModule("BossScream");
				break;
			case enState_Start:
				ImportModule("BossStart");
				break;
			}

			//PythonスクリプトのUpdate()関数を呼び出す。
			auto updateFunc = m_enemyPyModule.attr("Update");
			updateFunc();
		}

		void CBoss::InitStatus() {

			m_status.hp = c_bossMaxHP;
			m_status.attack = 10;
		}

		void CBoss::InitAnimationClip() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/Boss/walk.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/Boss/idle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_JumpAttack].Load("Assets/animData/Boss/attack.tka");
			m_animationClip[enAnim_JumpAttack].SetLoopFlag(false);
			m_animationClip[enAnim_SwipingAttack].Load("Assets/animData/Boss/swiping.tka");
			m_animationClip[enAnim_SwipingAttack].SetLoopFlag(false);
			m_animationClip[enAnim_Damage].Load("Assets/animData/damage.tka");
			m_animationClip[enAnim_Damage].SetLoopFlag(false);
			m_animationClip[enAnim_Death].Load("Assets/animData/Boss/death.tka");
			m_animationClip[enAnim_Death].SetLoopFlag(false);
			m_animationClip[enAnim_Scream].Load("Assets/animData/Boss/scream.tka");
			m_animationClip[enAnim_Scream].SetLoopFlag(false);
			m_animationClip[enAnim_Start].Load("Assets/animData/Boss/startAnim.tka");
			m_animationClip[enAnim_Start].SetLoopFlag(false);
		}

		void CBoss::AnimationUpdate() {

			float bossAnimBlendValue = c_bossAnimBlendValue;

			//登場時だけは補間をしない。
			if (m_isAnimStart) {

				bossAnimBlendValue = 0.0f;
				m_isAnimStart = false;
			}

			//各ステートに対応したアニメーションを再生する。
			switch (m_state) {
			case enState_Idle:
				m_modelRender->PlayAnimation(enAnim_Idle, bossAnimBlendValue);
				break;
			case enState_Walk:
				m_modelRender->PlayAnimation(enAnim_Walk, bossAnimBlendValue);
				break;
			case enState_JumpAttack:
				m_modelRender->PlayAnimation(enAnim_JumpAttack, bossAnimBlendValue);
				break;
			case enState_SwipingAttack:
				m_modelRender->PlayAnimation(enAnim_SwipingAttack, bossAnimBlendValue);
				break;
			case enState_Damage:
				m_modelRender->PlayAnimation(enAnim_Damage, bossAnimBlendValue);
				break;
			case enState_Death:
				m_modelRender->PlayAnimation(enAnim_Death, bossAnimBlendValue);
				break;
			case enState_Scream:
				m_modelRender->PlayAnimation(enAnim_Scream, bossAnimBlendValue);
				break;
			case enState_Start:
				m_modelRender->PlayAnimation(enAnim_Start, bossAnimBlendValue);
				break;
			}
		}

		void CBoss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
		{
			//キーの名前が「attack」の時。
			if (wcscmp(eventName, L"startAttack") == 0)
			{
				//攻撃中にする。
				m_triggerBox.ActivateAttack();
			}
			//キーの名前が「attack_end」の時。
			else if (wcscmp(eventName, L"endAttack") == 0)
			{
				//攻撃を終わる。
				m_triggerBox.DeactivateAttack();
			}
			//キーの名前が「attack」の時。
			else if (wcscmp(eventName, L"startRangeAttack") == 0)
			{
				//攻撃中にする。
				m_triggerBox.ActivateRangeAttack();

				auto mainCamera = FindGO<CMainCamera>(c_classNameMainCamera);

				//カメラを揺れ状態にする。
				mainCamera->ShakeCamera();
			}
			//キーの名前が「attack_end」の時。
			else if (wcscmp(eventName, L"endRangeAttack") == 0)
			{
				//攻撃を終わる。
				m_triggerBox.DeactivateRangeAttack();
			}
			else if (wcscmp(eventName, L"ready") == 0)
			{
				//ジャンプ攻撃の準備。
				//プレイヤーに伸びるベクトルを求める。
				m_vecToPlayer = m_player->GetPosition() - m_position;
			}
			else if (wcscmp(eventName, L"startJump") == 0) {

				//移動できる。
				m_canMove = true;
			}
			else if (wcscmp(eventName, L"endJump") == 0) {

				//移動できない。
				m_canMove = false;

				//衝撃SEを再生。
				CSoundManager::GetInstance()->Play(enSE_Impact);
			}
			else if (wcscmp(eventName, L"impact") == 0) {

				//衝撃SEを再生。
				CSoundManager::GetInstance()->Play(enSE_Impact);
			}
			else if (wcscmp(eventName, L"endShakeCamera") == 0) {

				auto mainCamera = FindGO<CMainCamera>(c_classNameMainCamera);
				
				//カメラを通常状態にする。
				mainCamera->SetNormalCamera();
			}
			else if (wcscmp(eventName, L"scream") == 0) {

				//咆哮SEを再生。
				CSoundManager::GetInstance()->Play(enSE_Scream);
			}
			else if (wcscmp(eventName, L"walk") == 0) {

				//ボス足音SEを再生。
				CSoundManager::GetInstance()->Play(enSE_Footsteps);
			}
			else if (wcscmp(eventName, L"scratch") == 0) {

				//引っ掻きSEを再生。
				CSoundManager::GetInstance()->Play(enSE_Scratch);
			}
		}

		void CBoss::CreateAttackCollision() {

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//コリジョンオブジェクトを作成。
			auto collisionObject = NewGO<CAttackCollision>(enPriority_Zeroth, c_classnameEnemyAttackCollision);

			//有効時間を設定。
			collisionObject->SetActiveTime(c_attackCollisionActiveTime);

			//ボックス状のコリジョンを作成。
			collisionObject->CreateBox(m_position, CQuaternion::Identity, c_bossAttackTriggerBoxSize);

			//剣のボーンのワールド行列をコリジョンに適用。
			collisionObject->SetWorldMatrix(swordBaseMatrix);
		}

		void CBoss::Move() {

			switch (m_state) {

			case enState_JumpAttack:
				//ジャンプ攻撃中の移動処理。
				JumpAttackMove();
				break;
			default:
				//通常の移動処理。
				MoveCommon();
				break;
			}
		}

		void CBoss::MoveCommon() {

			//x方向とz方向の移動速度を初期化。
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;

			//プレイヤーの座標を取得する。
			CVector3 toPlayerVec = m_player->GetPosition() - m_position;
			toPlayerVec.y = 0.0f;

			//正規化。
			toPlayerVec.Normalize();

			//歩き状態ならプレイヤーに一定速度で近づく。
			if (m_state == enState_Walk) {

				m_moveSpeed += toPlayerVec * 200.0f;
			}

			//重力をかける。
			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

			//座標を設定。
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}
		}

		void CBoss::JumpAttackMove() {

			//ジャンプ中じゃないなら終了。
			if (!m_canMove) { return; }

			//x方向とz方向の移動速度を初期化。
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;

			//求められたベクトルに向かって移動する。
			m_moveSpeed += m_vecToPlayer;

			//重力をかける。
			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

			//座標を設定。
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			//地面についているか判定。
			if (m_charaCon.IsOnGround()) {

				//地面についているなら下向きには力をかけない。
				m_moveSpeed.y = 0.0f;
			}
		}

		void CBoss::UpdateTriggerBox() {

			//トリガーボックスを更新。
			m_triggerBox.Update(m_position);
		}

		void CBoss::FindPlayer() {
 
			auto backGround = FindGO<CBackGround>(c_classNameBackGround);

			//ボスが登場しているなら
			if (backGround->CreatedBoss()) {

				//プレイヤーに気づいた。
				m_noticePlayer = true;
			}
		}
	}
}