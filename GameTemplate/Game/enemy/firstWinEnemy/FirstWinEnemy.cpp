#include "stdafx.h"
#include "FirstWinEnemy.h"
#include "../../AttackCollision.h"

namespace nsMyGame {

	namespace nsEnemy {

		extern CEnemy* g_pCurrentEnemy;

		bool CFirstWinEnemy::StartSub() {

			//IGameObjectに追加。
			m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);
		
			//tkmファイルとtksファイルを設定。
			m_modelRender->SetFilePathTkm(c_filePathTkmEnemy);
			m_modelRender->SetFilePathTks(c_filePathTksEnemy);

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
				20.0f,			//半径。
				100.0f,			//高さ。
				m_position		//座標。
			);

			//剣に取り付けられたボーンの番号を読み込む。
			auto swordBoneNum = m_modelRender->GetSkeleton()->FindBoneID(L"mixamorig5:LeftHand");

			//剣のボーンを取得。
			m_swordBone = m_modelRender->GetSkeleton()->GetBone(swordBoneNum);

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定のインスタンスを初期化。
			m_triggerBox.Create(m_position, m_rotation);

			//当たり判定の座標と回転を更新。
			m_triggerBox.UpdatePositionAndRotation(swordBaseMatrix);

			//当たり判定をしないように設定。
			m_triggerBox.Deactivate();

			return true;
		}

		void CFirstWinEnemy::UpdateSub() {
			
			// 現在更新処理を実行中のエネミーのアドレスを代入
			g_pCurrentEnemy = this;

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//当たり判定のワールド行列を更新。
			m_triggerBox.UpdatePositionAndRotation(swordBaseMatrix);
			
			//ステートに応じて読み込むPythonスクリプトを変える。
			switch (m_state) {
			case enState_Idle:
				ImportModule("EnemyIdle");
				break;
			case enState_Walk:
				ImportModule("EnemyMove");
				break;
			case enState_ThreeCombo:
				ImportModule("EnemyAttack");
				break;
			case enState_Damage:
				ImportModule("EnemyDamage");
				break;
			case enState_Death:
				ImportModule("EnemyDeath");
				break;
			case enState_AttackBreak:
				ImportModule("EnemyAttackBreak");
				break;
			case enState_Back:
				ImportModule("EnemyBack");
				break;
			}

			//PythonスクリプトのUpdate()関数を呼び出す。
			auto updateFunc = m_enemyPyModule.attr("Update");
			updateFunc();
		}

		void CFirstWinEnemy::InitStatus() {

			m_status.hp = 100;
			m_status.attack = 10;
		}

		void CFirstWinEnemy::InitAnimationClip() {

			//アニメーションクリップを設定。
			m_animationClip[enAnim_Walk].Load("Assets/animData/run.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].Load("Assets/animData/enemyIdle.tka");
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_ThreeCombo].Load("Assets/animData/threeCombo.tka");
			m_animationClip[enAnim_ThreeCombo].SetLoopFlag(false);
			m_animationClip[enAnim_Damage].Load("Assets/animData/damage.tka");
			m_animationClip[enAnim_Damage].SetLoopFlag(false);
			m_animationClip[enAnim_Death].Load("Assets/animData/death.tka");
			m_animationClip[enAnim_Death].SetLoopFlag(false);
			m_animationClip[enAnim_AttackBreak].Load("Assets/animData/attackBreak.tka");
			m_animationClip[enAnim_AttackBreak].SetLoopFlag(false);
			m_animationClip[enAnim_Back].Load("Assets/animData/back.tka");
			m_animationClip[enAnim_Back].SetLoopFlag(true);
		}

		void CFirstWinEnemy::AnimationUpdate() {

			//各ステートに対応したアニメーションを再生する。
			switch (m_state) {
			case enState_Idle:
				m_modelRender->PlayAnimation(enAnim_Idle, 0.8f);
				break;
			case enState_Walk:
				m_modelRender->PlayAnimation(enAnim_Walk, 0.4f);
				break;
			case enState_ThreeCombo:
				m_modelRender->PlayAnimation(enAnim_ThreeCombo, 0.4f);
				break;
			case enState_Damage:
				m_modelRender->PlayAnimation(enAnim_Damage, 0.4f);
				break;
			case enState_Death:
				m_modelRender->PlayAnimation(enAnim_Death, 0.4f);
				break;
			case enState_AttackBreak:
				m_modelRender->PlayAnimation(enAnim_AttackBreak, 0.4f);
				break;
			case enState_Back:
				m_modelRender->PlayAnimation(enAnim_Back, 0.4f);
				break;
			}
		}

		void CFirstWinEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
		{
			//キーの名前が「attack」の時。
			if (wcscmp(eventName, L"attack") == 0)
			{
				//攻撃中にする。
				m_triggerBox.Activate();
			}
			//キーの名前が「attack_end」の時。
			else if (wcscmp(eventName, L"attackEnd") == 0)
			{
				//攻撃を終わる。
				m_triggerBox.Deactivate();
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

		void CFirstWinEnemy::CreateAttackCollision() {

			//剣のボーンのワールド行列を取得。
			CMatrix swordBaseMatrix = m_swordBone->GetWorldMatrix();

			//コリジョンオブジェクトを作成。
			auto collisionObject = NewGO<CAttackCollision>(enPriority_Zeroth, c_classnameEnemyAttackCollision);
			
			//有効時間を設定。
			collisionObject->SetActiveTime(c_attackCollisionActiveTime);

			//ボックス状のコリジョンを作成。
			collisionObject->CreateBox(m_position, CQuaternion::Identity, c_attackTriggerBoxSize);

			//剣のボーンのワールド行列をコリジョンに適用。
			collisionObject->SetWorldMatrix(swordBaseMatrix);
		}

		void CFirstWinEnemy::Move() {

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

				m_moveSpeed += toPlayerVec * 240.0f;
			}
			//歩き状態ならプレイヤーに一定速度で近づく。
			if (m_state == enState_Back) {

				m_moveSpeed -= toPlayerVec * 120.0f;
				m_canRotate = true;
			}
			else {
				m_canRotate = false;
			}

			//3連続攻撃状態なら一定速度でプレイヤーに近づく。
			if (m_state == enState_ThreeCombo) {

				if (c_threeComboCoolTime - m_coolTime < 2.4f) {
					m_moveSpeed += toPlayerVec * 50.0f;
				}
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

		void CFirstWinEnemy::UpdateTriggerBox() {

			//トリガーボックスを更新。
			m_triggerBox.Update();
		}
	}
}