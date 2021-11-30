#include "stdafx.h"
#include "Player.h"
#include "../enemy/Enemy.h"
#include "../BackGround.h"

namespace nsMyGame {

	namespace nsPlayer {

		bool CPlayer::Start() {

			m_position = { 0.0f,200.0f,0.0f };

			m_status.hp = 100;
			m_status.attack = 10;

			//IGameObjectに追加。
			m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

			//tkmファイルとtksファイルを設定。
			m_modelRender->SetFilePathTkm(c_filePathTkmPlayer);
			m_modelRender->SetFilePathTks(c_filePathTksPlayer);

			//行動クラスを初期化。
			m_playerAction.Init(m_position, m_rotation, m_forward);

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

			return true;
		}


		void CPlayer::Update() {

			//移動処理。
			m_playerAction.Move(m_position, m_forward, m_playerState);

			//回転処理。
			if (m_playerState != enState_Rolling) {
				m_playerAction.Rotate(m_rotation);
			}

			//前方向を更新。
			UpdateForward();

			//アクション処理。
			m_playerAction.Action(m_playerState, m_isSelect);

			//アニメーション処理。
			m_playerAnimation.Update(*m_modelRender, m_playerState);

			//状態を更新。
			m_playerAction.Update(m_position, m_rotation, m_forward, m_playerState);

			//座標を設定。
			m_modelRender->SetPosition(m_position);

			//回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//ライトカメラを更新。
			LightCameraUpdate();
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

			auto backGround = FindGO<CBackGround>("backGround");

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

			CCamera::GetLightCamera()->SetUpdateProjMatrixFunc(CCamera::enUpdateProjMatrixFunc_Ortho);
		}
	}
}