#include "stdafx.h"
#include "Player.h"
#include "PlayerStateProcess.h"
#include "../AttackCollision.h"
#include "../enemy/Enemy.h"
#include "../BackGround.h"

namespace nsMyGame {

	namespace nsPlayer {

		namespace {
			constexpr float lightCameraDirection = 300.0f;
			constexpr float lightCameraFar = 800.0f;
		}
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

			//アニメーションに対応させる。
			//m_modelRender->InitAnimation(m_playerAnimation.GetAnimationClip(), m_playerAnimation.GetAnimationNum());

			//影を落とす。
			m_modelRender->SetShadowCasterFlag(true);

			//影を受ける。
			m_modelRender->SetShadowReceiverFlag(true);

			//初期化。
			m_modelRender->Init();

			//アニメーションクラスを初期化。
			m_playerAnimation.Init(*m_modelRender);

			//剣に取り付けられたボーンの番号を読み込む。
			int swordBoneNum = m_modelRender->GetSkeleton()->FindBoneID(L"mixamorig5:LeftHandRing4");

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

			//死んでいる状態かどうか判定。
			if (IsDeath()) { m_playerState = enState_Death; }

			//死んでいないなら
			if (!IsDeath()) {

				//移動処理。
				m_playerAction.Move(m_position, m_forward, m_playerState);

				//回転処理。
				m_playerAction.Rotate(m_rotation, m_forward, m_playerState);

				//前方向を更新。
				UpdateForward();

				//アクション処理。
				m_playerAction.Action(m_playerState, m_isSelect);

				//入力によって動いているなら
				if (m_playerAction.IsMove()) {

					//入力によって動いていることをアニメーション担当に伝える。
					m_playerAnimation.Move();
				}
			}

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//クールタイムを更新。
			m_playerAction.Update();

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//ライトカメラを更新。
			LightCameraUpdate();

			//選択状態を解除。
			m_isSelect = false;
		}

		void CPlayer::JudgeDamage(const CVector3& effectPos) {

			//ガード成功中、被弾中、死んだ状態なら終了。
			if (m_playerState == enState_GuardSuccess
				|| m_playerState == enState_Death
				|| m_playerState == enState_Damage
			) { return; }

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
			CVector3 m_lightCameraPos = m_lightCameraTar - *ligDirection * lightCameraDirection;

			//ライトカメラの情報を設定。
			CCamera::GetLightCamera()->SetPosition(m_lightCameraPos);
			CCamera::GetLightCamera()->SetTarget(m_lightCameraTar);
			CCamera::GetLightCamera()->SetFar(lightCameraFar);

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