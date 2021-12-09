#include "stdafx.h"
#include "AttackCollision.h"

namespace nsMyGame {

	CAttackCollision::~CAttackCollision() {

		//コリジョンを削除。
		m_ghostBox.Release();
	}

	bool CAttackCollision::Start() {

		//有効時間が設定されてないならエラー表示。
		if (m_activeTime <= 0.0f) {
			
			MessageBox(nullptr, L"当たり判定の有効時間が設定されていません。", L"警告", MB_OK);

			return false;
		}

		return true;
	}

	void CAttackCollision::CreateBox(const CVector3& pos, const CQuaternion& rot, const CVector3& size) {

		m_ghostBox.CreateBox(pos, rot, size);
	}

	void CAttackCollision::Update() {

		//有効タイムが切れていないならタイムを更新。
		if (m_activeTime > 0.0f) {

			m_activeTime -= GameTime().GameTimeFunc().GetFrameDeltaTime();
		}
		//タイムが切れたらこの当たり判定を削除。
		else {
			
			DeleteGO(this);
		}
	}
}