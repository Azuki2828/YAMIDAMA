#include "stdafx.h"
#include "Player.h"

namespace nsMyGame {

	namespace nsPlayer {

		CPlayer* g_pCurrentPlayer = nullptr;

		void Normal() {
			g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Idle, c_animationInterpolateTime);
		}

		void Yoi() {
			g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Walk, c_animationInterpolateTime);
		}

		void Death() {
			g_pCurrentPlayer->GetModelRender()->PlayAnimation(enAnim_Death, c_animationInterpolateTime);
		}

		int GetYoiParam() {

			return g_pCurrentPlayer->GetYoiParam();
		}

		void ChangeState(int state) {

			g_pCurrentPlayer->SetState(state);
		}

		void Move() {
			g_pCurrentPlayer->Move();
		}

		void Drink() {

			if (g_pad[0]->IsTrigger(enButtonA)) {
				int yoiParam = g_pCurrentPlayer->GetYoiParam();
				yoiParam += 10;
				g_pCurrentPlayer->SetYoiParam(yoiParam);
			}
		}

		void LightCameraUpdate() {
			g_pCurrentPlayer->LightCameraUpdate();
		}

		void FontUpdate() {
			g_pCurrentPlayer->FontUpdate();
		}

		PYBIND11_MODULE(Game, m) {
			m.def("Normal", &Normal);
			m.def("Yoi", &Yoi);
			m.def("Death", &Death);
			m.def("Move", &Move);
			m.def("Drink", &Drink);
			m.def("GetYoiParam", &GetYoiParam);
			m.def("ChangeState", &ChangeState);
			m.def("LightCameraUpdate", &LightCameraUpdate);
			m.def("FontUpdate", &FontUpdate);
		}






		



		bool CPlayer::Start() {

			m_modelRender = NewGO<CModelRender>(0);
			m_modelRender->SetFilePathTkm("Assets/modelData/knight.tkm");
			m_modelRender->SetFilePathTks("Assets/modelData/knight.tks");
			m_modelRender->SetFilePathTkm("Assets/unityChanBeer.tkm");
			m_modelRender->SetFilePathTks("Assets/unityChanBeer.tks");
			m_animationClip[enAnim_Walk].Load("Assets/animData/walk.tka");
			m_animationClip[enAnim_Idle].Load("Assets/animData/Idle.tka");
			m_animationClip[enAnim_Death].Load("Assets/animData/KneelDown.tka");
			m_animationClip[enAnim_Walk].SetLoopFlag(true);
			m_animationClip[enAnim_Idle].SetLoopFlag(true);
			m_animationClip[enAnim_Death].SetLoopFlag(false);
			m_modelRender->InitAnimation(m_animationClip, enAnimNum);
			m_modelRender->SetAnimFlg(true);
			m_modelRender->SetShadowCasterFlag(true);
			m_modelRender->SetShadowReceiverFlag(true);
			m_modelRender->SetModelUpAxis(EnModelUpAxis::enModelUpAxisY);
			m_modelRender->Init();

			m_fontRender = NewGO<nsFont::CFontRender>(0);
			m_fontRender->Init(L"酔い度：", { -550.0f,310.0f });	//場所
			m_fontRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });			//赤色
			m_fontRender->SetShadowParam(true, 1.0f, CVector4::Black);

			m_fontRender2 = NewGO<nsFont::CFontRender>(0);
			m_fontRender2->Init(L"A：飲む", { 450.0f,310.0f });	//場所
			m_fontRender2->SetColor({ 1.0f,0.0f,0.0f,1.0f });			//赤色
			m_fontRender2->SetShadowParam(true, 1.0f, CVector4::Black);


			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				20.0f,			//半径。
				100.0f,			//高さ。
				m_position		//座標。
			);
			return true;
		}

		void CPlayer::StateUpdate() {

			//現在のインスタンスを代入。
			g_pCurrentPlayer = this;

			//Pythonスクリプトをロードする。
			switch (m_playerState) {
			case EnPlayerState::enState_Normal:

				//通常
				ImportModule("PlayerNormal");
				break;
			case EnPlayerState::enState_Yoi:

				//ほろよい
				ImportModule("PlayerYoi");
				break;
			case EnPlayerState::enState_Death:

				//急性アルコール中毒
				ImportModule("PlayerDeath");
				break;
			}

			//Pythonスクリプトの中のUpdate()関数を呼び出す。
			auto updateFunc = m_playerPyModule.attr("Update");
			updateFunc();
		}


		void CPlayer::Update() {

			//ステートによって読み込むPythonスクリプトを切り替える。
			StateUpdate();
		}












		void CPlayer::LightCameraUpdate() {

			//ライトカメラの情報を更新
			CVector3 m_lightCameraTar = m_position;
			CVector3 m_lightCameraPos = m_lightCameraTar;

			//ライトカメラの座標を設定
			m_lightCameraPos.y += 300.0f;
			m_lightCameraPos.z += 300.0f;

			//ライトカメラの情報を設定
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
			CCamera::GetLightCamera()->SetUp(CameraUp);
			CCamera::GetLightCamera()->Update();
		}

		void CPlayer::Move() {

			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;
			
			//このフレームの移動量を求める。
			//左スティックの入力量を受け取る。
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
			//XZ成分の移動速度をクリア。
			m_moveSpeed += cameraForward * lStick_y * 500.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 500.0f;		//右方向への移動速度を加算。
			if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら
				//&& m_charaCon.IsOnGround()  //かつ、地面に居たら
				) {
				//ジャンプする。
				m_moveSpeed.y = 400.0f;	//上方向に速度を設定して、
			}

			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
			//キャラクターコントローラーを使用して、座標を更新。

			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			if (m_charaCon.IsOnGround()) {
				//地面についた。
				m_moveSpeed.y = 0.0f;
			}

			//座標を設定。
			m_modelRender->SetPosition(m_position);
		}

		void CPlayer::FontUpdate() {

			wchar_t time[64];
			swprintf_s(time, L"酔い度：%d", m_yoiParam);

			if (m_yoiParam >= 60 && m_yoiParam < 100) {
				m_color = { 1.0f,0.0f,0.0f,1.0f };
			}
			else if (m_yoiParam >= 100) {
				switch (m_colorState) {
				case en1:
					m_color.y += 0.05f;
					if (m_count == 20) {
						m_colorState = en2;
						m_count = 0;
					}
					break;
				case en2:
					m_color.x -= 0.05f;
					if (m_count == 20) {
						m_colorState = en3;
						m_count = 0;
					}
					break;
				case en3:
					m_color.z += 0.05f;
					if (m_count == 20) {
						m_colorState = en4;
						m_count = 0;
					}
					break;
				case en4:
					m_color.y -= 0.05f;
					if (m_count == 20) {
						m_colorState = en5;
						m_count = 0;
					}
					break;
				case en5:
					m_color.x += 0.05f;
					if (m_count == 20) {
						m_colorState = en6;
						m_count = 0;
					}
					break;
				case en6:
					m_color.z -= 0.05f;
					if (m_count == 20) {
						m_colorState = en1;
						m_count = 0;
					}
					break;
				}
				m_count++;
				swprintf_s(time, L"酔い度：急性アルコール中毒");
			}
			m_fontRender->SetText(time);
			m_fontRender->SetColor(m_color);
		}
	}
}