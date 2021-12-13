#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameTitle.h"

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
#ifdef MASTER
		Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;./");
#else
#ifdef MY_DEBUG
		Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;../x64/Debug/;./");
#else
		Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;../x64/Release/;./");
#endif
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
		CGameObjectManager::CreateInstance();				//ゲームオブジェクト管理クラス
		CPhysicsWorld::CreateInstance();					//物理ワールド
		nsLight::CLightManager::CreateInstance();			//ライト管理クラス
		CCamera::CreateLightCamera();						//ライトカメラ
		CRenderingEngine::CreateRenderingEngine();			//レンダリングエンジン
		EffectEngine::CreateInstance();						//エフェクトエンジン
		NewGO<Fade>(enPriority_Zeroth, c_classNameFade);	//フェード

		//ワイヤーフレーム表示をONにする。
		//CPhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

		////////////////////////////////////////////////
		// 初期化を行うコードを書くのはここまで
		////////////////////////////////////////////////
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		
		NewGO<CGameTitle>(enPriority_Zeroth);
		//NewGO<CGameMain>(enPriority_Zeroth);

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

