#include "stdafx.h"
#include "system/system.h"
#include "Enemy.h"
#include "Player.h"



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
	Py_SetPath(L"./Python37_64/DLLs;./Python37_64/Lib;./EnemyState;../x64/Debug/");
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

	wchar_t* program;

	// python初期化。
	InitPython(program);

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// とりあえずテストで敵を追加。
	NewGO<Enemy>(0, nullptr);
	NewGO<Player>(0);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		//sprite[1].Update(pos[1], Quaternion::Identity, Vector3::One);
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();

	// pythonの終了処理。
	FinalPython(program);

	return 0;
}

