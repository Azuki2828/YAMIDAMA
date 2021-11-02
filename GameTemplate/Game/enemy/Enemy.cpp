#include "stdafx.h"
#include "Enemy.h"

namespace nsMyGame {

	namespace nsEnemy {

		CEnemy* g_pCurrentEnemy = nullptr;

		void ChangeState(int stateNum) {

			g_pCurrentEnemy->ChangeState(stateNum);
		}

		float GetLengthToPlayer() {

			return g_pCurrentEnemy->GetLengthToPlayer();
		}

		void Move() {

			g_pCurrentEnemy->Move();
		}

		void Rotate() {

			g_pCurrentEnemy->Rotate();
		}

		void SetCoolTime(const float coolTime) {

			g_pCurrentEnemy->SetCoolTime(coolTime);
		}

		float GetCoolTime() {

			return g_pCurrentEnemy->GetCoolTime();
		}

		void AnimationUpdate() {

			g_pCurrentEnemy->AnimationUpdate();
		}

		bool IsDeath() {

			return g_pCurrentEnemy->IsDeath();
		}

		void Delete() {

			g_pCurrentEnemy->Delete();
		}


		PYBIND11_MODULE(Game, m) {
			m.def("ChangeState", &ChangeState);
			m.def("GetLengthToPlayer", &GetLengthToPlayer);
			m.def("Move", &Move);
			m.def("Rotate", &Rotate);
			m.def("SetCoolTime", &SetCoolTime);
			m.def("GetCoolTime", &GetCoolTime);
			m.def("AnimationUpdate", &AnimationUpdate);
			m.def("IsDeath", &IsDeath);
			m.def("Delete", &Delete);
		}


		bool CEnemy::Start()
		{
			m_player = FindGO<nsPlayer::CPlayer>("player");

			//キャラクターコントローラーを初期化。
			m_charaCon.Init(
				20.0f,			//半径。
				100.0f,			//高さ。
				m_position		//座標。
			);

			//派生クラスのStartSub()関数の結果を返す。
			return StartSub();
		}

		CEnemy::~CEnemy() {
			if (m_modelRender != nullptr) {
				DeleteGO(m_modelRender);
			}
		}

		void CEnemy::Update()
		{
			//派生クラスのUpdateSub()関数を呼び出す。
			UpdateSub();

			UpdateForward();


			m_modelRender->SetPosition(m_position);
			m_modelRender->SetRotation(m_rotation);

			if (m_coolTime > 0.0f) {

				m_coolTime -= g_gameTime->GetFrameDeltaTime();
			}
			else {
				m_coolTime = 0.0f;
			}
		}
		void CEnemy::Render(CRenderContext& rc)
		{

		}

		void CEnemy::Rotate() {

			//移動ボタンが入力されていなかったら
			if (fabsf(m_moveSpeed.x) < 0.001f
				&& fabsf(m_moveSpeed.z) < 0.001f) {
				//このフレームではキャラは移動していないので旋回する必要はない。
				return;
			}

			//回転角度を求める。
			float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);

			//回転を設定。
			m_rotation.SetRotationY(-angle);
		}

		void CEnemy::UpdateForward() {

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
	}
}