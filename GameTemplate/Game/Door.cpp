#include "stdafx.h"
#include "Door.h"
#include "player/Player.h"
#include "AppearSprite.h"

namespace nsMyGame{

	bool CDoor::Start() {

		//モデルを初期化。
		InitModel();

		//メッセージウィンドウを初期化。
		InitSpriteAndText();

		return true;
	}

	void CDoor::Update() {

		//開かないドアなら更新しない。
		if (m_isObj) { return; }

		//ドアを開ける判定をして開けられたら開ける。
		JudgeAndExecuteOpenDoor(m_doorRotNum);

		//座標を更新。
		m_modelRender->SetPosition(m_position);

		//回転を更新。
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

		//メッセージウィンドウを検索。
		auto lockSprite = FindGO<CAppearSprite>(c_classNameAppearSprite);

		//メッセージウィンドウが表示されているなら
		if (lockSprite != nullptr) {

			//選択スプライトを非表示にする。
			m_doorSprite->Deactivate();
			m_text->Deactivate();

			//透明度を初期化。
			m_doorSpriteTranslucent = c_translucentValue_Zero.w;

			//処理はここで終了。
			return;
		}

		//プレイヤーとの距離が一定以下かつまだ開いてない
		if (vecToPlayer.Length() <= c_distanceForOpenDoor && !IsOpened()) {
			 
			//スプライトとテキストを徐々に出現させる。
			AppearSpriteAndText();

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

						//確認ウィンドウを生成。
						auto getSprite = NewGO<CAppearSprite>(enPriority_Zeroth);
						getSprite->SetText(L"Key used.  A:OK");
						getSprite->SetTextPosition(c_textPosition_getKey);
					}
					//鍵をもっていない
					else {

						//鍵を所持していない確認のウィンドウを生成。
						auto getSprite = NewGO<CAppearSprite>(enPriority_Zeroth, c_classNameAppearSprite);
						getSprite->SetText(L"It's locked.  A:OK");
						getSprite->SetTextPosition(c_textPosition_getKey);
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

			//だんだんスプライトが消えるようにする。
			DisappearSpriteAndText();
		}
	}

	void CDoor::UpdateRotation(unsigned int& rotNum) {

		//ドアを回転させるための回数が0より大きいなら
		if (rotNum > 0) {

			//回転を設定。
			m_rotation.AddRotationY(CMath::DegToRad(c_openDoorRotNum));

			//モデルの回転を更新。
			m_modelRender->SetRotation(m_rotation);

			//回数を減らす。
			rotNum--;
		}
	}

	void CDoor::AppearSpriteAndText() {

		//開くスプライトを表示。
		m_doorSprite->Activate();
		//テキストを表示。
		m_text->Activate();

		//だんだんスプライトが現れるようにする。
		if (m_doorSpriteTranslucent < c_translucentValue_Max.w) {

			//テキストカラーを設定。
			float textColor = m_doorSpriteTranslucent;
			m_text->SetColor({ textColor ,textColor ,textColor,m_doorSpriteTranslucent });

			//スプライトの透明度を設定。
			m_doorSpriteTranslucent += GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
			m_doorSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_doorSpriteTranslucent });
		}
	}

	void CDoor::DisappearSpriteAndText() {

		if (m_doorSpriteTranslucent > c_translucentValue_Zero.w) {

			//テキストカラーを設定。
			float textColor = m_doorSpriteTranslucent;
			m_text->SetColor({ textColor,textColor,textColor,m_doorSpriteTranslucent });

			//スプライトの透明度を設定。
			m_doorSpriteTranslucent -= GameTime().GameTimeFunc().GetFrameDeltaTime() * 5.0f;
			m_doorSprite->SetMulColor({ 1.0f,1.0f,1.0f, m_doorSpriteTranslucent });
		}
		//開くスプライトを非表示。
		//テキストを非表示。
		else {
			m_doorSpriteTranslucent = c_translucentValue_Zero.w;
			m_doorSprite->Deactivate();
			m_text->Deactivate();
		}
	}

	void CDoor::InitModel() {

		//モデルを生成。
		m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

		//tkmのファイルパスを設定。
		m_modelRender->SetFilePathTkm(c_filePathTkmDoor);

		//シャドウキャスターフラグを設定。
		m_modelRender->SetShadowCasterFlag(true);

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
	}

	void CDoor::InitSpriteAndText() {

		//テキストのスプライトを初期化。
		m_doorSprite = NewGO<CSpriteRender>(enPriority_Zeroth);
		m_doorSprite->Init(c_filePathTextSprite, c_textSpriteWH.x, c_textSpriteWH.y);
		m_doorSprite->SetPosition(c_textSpritePosition);
		m_doorSprite->SetScale(c_textSpriteSize);

		//非表示に設定。
		m_doorSprite->Deactivate();

		//テキストを設定。
		m_text = NewGO<nsFont::CFontRender>(enPriority_Zeroth);
		m_text->Init(L"A: Open");
		m_text->SetPosition(c_textPosition);
		m_text->SetScale(c_textSize);
		m_text->SetColor(CVector4::White);

		//非表示に設定。
		m_text->Deactivate();
	}
}