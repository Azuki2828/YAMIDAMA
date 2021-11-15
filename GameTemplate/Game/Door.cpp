#include "stdafx.h"
#include "Door.h"
#include "player/Player.h"

namespace nsMyGame{

	bool CDoor::Start() {

		//モデルを生成。
		m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

		//tkmのファイルパスを設定。
		m_modelRender->SetFilePathTkm(c_filePathTkmDoor);

		//シャドウキャスターフラグを設定。
		m_modelRender->SetShadowReceiverFlag(true);

		//座標を設定。
		m_modelRender->SetPosition(m_position);

		//回転を設定。
		m_modelRender->SetRotation(m_rotation);

		//拡大を設定。
		m_modelRender->SetScale(m_scale);

		//モデルを初期化。
		m_modelRender->Init();

		//静的物理オブジェクトを初期化。
		m_physicsStaticObject.CreateFromModel(
			*m_modelRender->GetModel(),
			m_modelRender->GetModel()->GetWorldMatrix()
		);

		//静的物理オブジェクトを更新。
		m_physicsStaticObject.GetRigidBody().SetPositionAndRotation(m_position, m_rotation);

		return true;
	}

	void CDoor::Update() {

		//オブジェクトなら更新しない。
		if (m_isObj) {
			return;
		}

		//ドアを開ける判定をして開けられたら開ける。
		JudgeAndExecuteOpenDoor(m_doorRotNum);

		//ドアの回転を更新。
		UpdateRotation(m_doorRotNum);

		//静的物理オブジェクトを更新。
		m_physicsStaticObject.GetRigidBody().SetPositionAndRotation(m_position, m_rotation);
	}

	void CDoor::JudgeAndExecuteOpenDoor(unsigned int& rotNum) {

		//プレイヤーを検索。
		auto player = FindGO<nsPlayer::CPlayer>(c_classNamePlayer);

		//プレイヤーの座標を取得。
		CVector3 playerPos = player->GetPosition();

		//プレイヤーに伸びるベクトルを計算。
		CVector3 vecToPlayer = playerPos - m_position;

		//プレイヤーとの距離が一定以下かつまだ開いてない
		if (vecToPlayer.Length() <= c_distanceForOpenDoor && !IsOpened()) {

			//プレイヤーが何かを選んでいる状態にする。
			player->SetSelectFlag(true);

			//Aボタンが入力された
			if (g_pad[0]->IsTrigger(enButtonA)) {

				//鍵がかかっている？
				if (IsLocked()) {

					//プレイヤーが鍵を持っている？
					if (player->HasKey()) {

						//ロックを外す。
						m_isLocked = false;

						//ドアを回転させるための回数を設定。
						rotNum = c_openDoorRotValue / c_openDoorRotNum;

						//鍵を消費する。
						player->ConsumeKey();

						//開いた状態に設定。
						m_isOpened = true;
					}
					//鍵をもっていない
					else {
						/////////////////////////////////////////////
						//鍵をもってないよ！のテキストを表示させる。
						/////////////////////////////////////////////
					}
				}
				//鍵はかかっていない
				else {

					//ドアを回転させるための回数を設定。
					rotNum = c_openDoorRotValue / c_openDoorRotNum;

					//開いた状態に設定。
					m_isOpened = true;
				}
			}
		}
		else {

			//プレイヤーが何も選んでいない状態にする。
			player->SetSelectFlag(false);
		}
	}

	void CDoor::UpdateRotation(unsigned int& rotNum) {

		//ドアを回転させるための回数が0より大きいなら
		if (rotNum > 0) {

			//回転を設定。
			m_rotation.AddRotationY(CMath::DegToRad(c_openDoorRotNum));

			//モデルに回転を設定。
			m_modelRender->SetRotation(m_rotation);

			//回数を減らす。
			rotNum--;
		}
	}
}