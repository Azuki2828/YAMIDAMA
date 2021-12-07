#include "stdafx.h"
#include "system/system.h"
#include "enemy/GoteWinEnemy/GoteWinEnemy.h"
#include "enemy/FirstWinEnemy/FirstWinEnemy.h"
#include "player/Player.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "Door.h"
#include "item/Item.h"

namespace nsMyGame {

	BOOL APIENTRY DllMain(HMODULE hModule,
		DWORD  ul_reason_for_call,
		LPVOID lpReserved
	)
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
		return TRUE;
	}

	/**
	 * @brief Pythonの初期化。
	*/
	void InitPython(wchar_t*& program)
	{
		program = Py_DecodeLocale("MyGame", nullptr);
		// モジュールのパスを設定。
#ifdef MY_DEBUG
		Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;../x64/Debug/");
#else
		Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;../x64/Release/");
#endif
		Py_SetProgramName(program);
		Py_Initialize();
	}
	/**
	 * @brief Pythonの終了処理。
	*/
	void FinalPython(wchar_t* program)
	{
		Py_Finalize();
		PyMem_RawFree(program);
	}
	///////////////////////////////////////////////////////////////////
	// ウィンドウプログラムのメイン関数。
	///////////////////////////////////////////////////////////////////
	extern "C" __declspec(dllexport) int WINAPI AppMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
	{
		//ゲームの初期化。
		InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));


		// pythonを初期化。
		wchar_t* program;
		InitPython(program);

		//////////////////////////////////////
		// ここから初期化を行うコードを記述する。
		//////////////////////////////////////

		

		//様々なインスタンスを作成する。
		CGameObjectManager::CreateInstance();		//ゲームオブジェクト管理クラス
		CPhysicsWorld::CreateInstance();			//物理ワールド
		nsLight::CLightManager::CreateInstance();	//ライト管理クラス
		CCamera::CreateLightCamera();				//ライトカメラ
		CRenderingEngine::CreateRenderingEngine();	//レンダリングエンジン
		EffectEngine::CreateInstance();				//エフェクトエンジン

		//ワイヤーフレーム表示をONにする。
		CPhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

		////////////////////////////////////////////////
		// 初期化を行うコードを書くのはここまで
		////////////////////////////////////////////////
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		
		// とりあえずテストで敵を追加。
		auto fEnemy = NewGO<nsEnemy::CFirstWinEnemy>(0, c_classNameEnemy);
		fEnemy->SetPosition({ 500.0f,500.0f,500.0f });
		//NewGO<nsEnemy::CGoteWinEnemy>(0, "Enemy");
		
		NewGO<nsPlayer::CPlayer>(0, c_classNamePlayer);
		NewGO<CBackGround>(0, c_classNameBackGround);
		NewGO<CMainCamera>(0);

		
		//CVector3 ligColor, ligDir;
		//ligColor.x = 0.6f;
		//ligColor.y = 0.6f;
		//ligColor.z = 0.6f;
		//ligDir.x = 1.0f;
		//ligDir.y = -1.0f;
		//ligDir.z = -1.0f;
		//ligDir.Normalize();
		//g_sceneLight->SetDirectionLight(0, ligDir, ligColor);


		/*CSpriteRender* m_spriteRender = NewGO<CSpriteRender>(0);
		m_spriteRender->Init("Assets/image/beer.dds", 1280.0f, 720.0f);
		m_spriteRender->SetPosition({ -400.0f,-200.0f,0.0f });
		m_spriteRender->SetScale({ 0.2f,0.2f,0.2f });
		m_spriteRender->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });*/

		// ここからゲームループ。
		while (DispatchWindowMessage())
		{
			//レンダリング開始。
			g_engine->BeginFrame();

			//////////////////////////////////////
			//ここから絵を描くコードを記述する。
			//////////////////////////////////////

			//ゲームオブジェクトを更新。
			CGameObjectManager::GetInstance()->ExecuteUpdate();
			EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

			//ライトを更新。
			nsLight::CLightManager::GetInstance()->Update();

			//描画。
			CRenderingEngine::GetInstance()->Render();
			////////////////////////////////////////////////
			//絵を描くコードを書くのはここまで！！！
			////////////////////////////////////////////////

			//レンダリング終了。
			g_engine->EndFrame();
		}

		//ゲームオブジェクトマネージャーを削除。
		CGameObjectManager::DeleteInstance();

		// pythonの終了処理。
		FinalPython(program);

		return 0;
	}
}

