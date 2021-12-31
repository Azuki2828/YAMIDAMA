#include "stdafx.h"
#include "PlayerStateProcess.h"
#include "Player.h"
#include "../AttackCollision.h"
#include "../enemy/Enemy.h"
#include "../BackGround.h"

namespace nsMyGame {

	namespace nsPlayer {

		bool CPlayer::Start() {

			//初期座標を設定。
			m_position = c_playerStartPosition;

			//初期回転を設定。
			CQuaternion quaternion = CQuaternion::Identity;
			quaternion.SetRotationDegY(180.0f);
			m_rotation = quaternion;

			//ステータスを初期化。
			InitStatus();

			//IGameObjectに追加。
			m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

			//tkmファイルとtksファイルを設定。
			m_modelRender->SetFilePathTkm(c_filePathTkmPlayer);
			m_modelRender->SetFilePathTks(c_filePathTksPlayer);

			//アニメーションイベント用の関数を設定する。
			m_modelRender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {

				m_playerAction.OnAnimationEvent(clipName, eventName);
			});

			//アニメーションクラスを初期化。
			m_playerAnimation.Init();

			//アニメーションを初期化。
			m_modelRender->InitAnimation(m_playerAnimation.GetAnimationClip(), m_playerAnimation.GetAnimationNum());

			//影を落とす。
			m_modelRender->SetShadowCasterFlag(true);

			//影を受ける。
			m_modelRender->SetShadowReceiverFlag(true);

			//初期化。
			m_modelRender->Init();

			//剣に取り付けられたボーンの番号を読み込む。
			int swordBoneNum = m_modelRender->GetSkeleton()->FindBoneID(L"mixamorig5:LeftHand");

			//剣のボーンを取得。
			Bone* swordBone = m_modelRender->GetSkeleton()->GetBone(swordBoneNum);

			//行動クラスを初期化。
			m_playerAction.Init(m_position, m_rotation, m_forward, swordBone);
			return true;
		}

		void CPlayer::OnDestroy() {

			DeleteGO(m_modelRender);
		}

		void CPlayer::Update() {

			//更新処理。
			switch (m_playerState) {

			case enState_Death:				//死んでいる状態

				IsDeathStateProcess();
				break;
			case enState_Rolling:			//ローリング中

				IsRollingStateProcess();
				break;
			case enState_Damage:			//被弾中

				IsDamagedStateProcess();
				break;
			case enState_Guard:				//ガード中
				IsGuardStateProcess();
				break;
			case enState_Attack:			//攻撃中
				IsAttackStateProcess();
				break;
			default:						//通常処理

				CommonStateProcess();
				break;
			}
		}

		void CPlayer::JudgeDamage(const CVector3& effectPos) {

			//ガード成功中なら終了。
			if (m_playerState == enState_GuardSuccess || m_playerState == enState_Death) { return; }

			//ガードしたならガード成功状態に。
			if (m_playerState == enState_Guard) {

				m_playerState = enState_GuardSuccess;
				m_playerAction.GuardSuccess();

				//ガードSEを再生。
				
				CSoundManager::GetInstance()->Play(enSE_Guard);

				//処理はここで終了。
				return;
			}

			//ダメージを受ける。
			ReceiveDamage(effectPos);
		}

		void CPlayer::UpdateForward() {

			//回転行列を計算。
			auto mRot = CMatrix::Identity;
			mRot.MakeRotationFromQuaternion(m_rotation);

			//前方向を設定。
			m_forward.x = mRot.m[2][0];
			m_forward.y = mRot.m[2][1];
			m_forward.z = mRot.m[2][2];

			//正規化。
			m_forward.Normalize();
		}

		void CPlayer::LightCameraUpdate() {

			//背景クラスを検索。
			auto backGround = FindGO<CBackGround>(c_classNameBackGround);

			//ライトカメラの情報を更新。
			CVector3 m_lightCameraTar = m_position;
			auto ligDirection = backGround->GetSunDirectionLight()->GetLigDirection();
			CVector3 m_lightCameraPos = m_lightCameraTar - *ligDirection * 300.0f;

			//ライトカメラの情報を設定。
			CCamera::GetLightCamera()->SetPosition(m_lightCameraPos);
			CCamera::GetLightCamera()->SetTarget(m_lightCameraTar);
			CCamera::GetLightCamera()->SetFar(800.0f);

			//カメラの上方向を求める。
			CVector3 Vec_x = CVector3::AxisX;
			CVector3 TarPos = CCamera::GetLightCamera()->GetTarget() - CCamera::GetLightCamera()->GetPosition();
			TarPos.Normalize();
			CVector3 CameraUp;
			CameraUp.Cross(TarPos, Vec_x);
			CameraUp.Normalize();

			//上方向を設定。
			CCamera::GetLightCamera()->SetUp(CameraUp);

			//ライトカメラを更新。
			CCamera::GetLightCamera()->Update();

			//平行投影に設定。
			CCamera::GetLightCamera()->SetUpdateProjMatrixFunc(CCamera::enUpdateProjMatrixFunc_Ortho);
		}
	}
}