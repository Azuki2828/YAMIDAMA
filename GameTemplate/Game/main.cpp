#include "stdafx.h"
#include "system/system.h"
#include "Enemy.h"
#include "Player.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "PostEffect.h"


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
	LightManager::CreateInstance();
	Camera::CreateLightCamera();
	RenderingEngine::CreateRenderingEngine();
	
	RenderingEngine::GetInstance()->Init();

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// とりあえずテストで敵を追加。
	//NewGO<Enemy>(0, nullptr);
	NewGO<Player>(0,"player");
	NewGO<BackGround>(0);
	NewGO<MainCamera>(0);

	DirectionLight* dirLight = NewGO<DirectionLight>(0);
	dirLight->SetLigDirection({0.0f,-1.0f,-1.0f});
	dirLight->SetLigColor({5.0f,5.0f,5.0f});

	PointLight* poiLight = NewGO<PointLight>(0);
	poiLight->SetPosition({ 100.0f,100.0f,0.0f });
	poiLight->SetColor({ 5.0f,0.0f,0.0f });
	poiLight->SetRange(200.0f);
	poiLight->SetAffectPowParam(3.0f);

	PointLight* poiLight2 = NewGO<PointLight>(0);
	poiLight2->SetPosition({ -50.0f,100.0f,0.0f });
	poiLight2->SetColor({ 0.0f,0.0f,5.0f });
	poiLight2->SetRange(200.0f);
	poiLight2->SetAffectPowParam(3.0f);

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
		LightManager::GetInstance()->Update();

		RenderingEngine::GetInstance()->Render();
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

