#include "stdafx.h"
#include "Door.h"
#include "player/Player.h"

namespace nsMyGame{

	bool CDoor::Start() {

		//モデルを生成。
		m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

		//tkmのファイルパスを設定。
		m_modelRender->SetFilePathTkm(c_filePathTkmDoor);

		//座標を設定。
		m_modelRender->SetPosition(m_position);

		//回転を設定。
		m_modelRender->SetRotation(m_rotation);

		//拡大を設定。
		m_modelRender->SetScale(m_scale);

		//モデルを初期化。
		m_modelRender->Init();

		return true;
	}

	void CDoor::Update() {

		//ドアを回転させるための回数を初期化。
		static unsigned int doorRotNum = 0;

		//プレイヤーを検索。
		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

		//プレイヤーの座標を取得。
		CVector3 playerPos = player->GetPosition();

		//プレイヤーに伸びるベクトルを計算。
		CVector3 vecToPlayer = playerPos - m_position;

		//もしプレイヤーとの距離が一定以下かつAボタンが入力されたら
		if (vecToPlayer.Length() <= c_distanceForOpenDoor && g_pad[0]->IsTrigger(enButtonA)) {

			//もし鍵がかかっているなら
			if (IsLocked()) {

				//プレイヤーが鍵を持っているなら。
				if (player->HasKey()) {

					//ロックを外す。
					m_isLocked = false;

					//ドアを回転させるための回数を設定。
					doorRotNum = c_openDoorRotValue / c_openDoorRotNum;

					//鍵を消費する。
					player->ConsumeKey();
				}
			}
			//鍵をもっていないなら
			else {
				/////////////////////////////////////////////
				//鍵をもってないよ！のテキストを表示させる。
				/////////////////////////////////////////////
			}
		}

		//ドアを回転させるための回数が0より大きいなら
		if (doorRotNum > 0) {

			//回転を設定。
			m_rotation.SetRotationDegY(static_cast<float>(c_openDoorRotNum));

			//モデルに回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//回数を減らす。
			doorRotNum--;
		}
	}
}