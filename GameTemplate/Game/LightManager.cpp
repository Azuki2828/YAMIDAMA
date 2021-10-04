#include "stdafx.h"
#include "LightManager.h"


LightManager* LightManager::m_lightManager = nullptr;

void LightManager::AddLight(LightBase* light) {

	//もし規定されている数に達していたら置けない
	if (m_directionLights.size() >= c_maxDirectionLightNum) {
		MessageBoxA(nullptr, "これ以上ディレクションライトは置けません", "エラー", MB_OK);
		return;
	}

	//クラスの名前を調べる
	const std::type_info& typeInfo = typeid(*light);

	//ディレクションライトだったら
	if (typeInfo == typeid(DirectionLight)) {

		//登録済みか調べる。
		auto findIt = std::find(m_directionLights.begin(), m_directionLights.end(), light->GetLigData());
		if (findIt == m_directionLights.end()) {
			//新規登録。
			m_directionLights.push_back(reinterpret_cast<DirectionLigData*>(light->GetLigData()));

			int a = 0;
			std::list<DirectionLigData*>::iterator itr;
			itr = m_directionLights.begin();

			//for (int i = 0; i < MAX_DIRECTION_LIGHT; i++) {
			//	m_ligData.m_directionLigData[i] = nullptr;
			//}
			for (auto itr = m_directionLights.begin(); itr != m_directionLights.end(); ++itr) {

				m_ligData.directionLigData[a] = *(*itr);
				a++;
			}
		}
		else {
			//既に登録されている。
			MessageBoxA(nullptr, "既に登録されています", "エラー", MB_OK);
			return;
		}
	}
	//ポイントライトだったら
	else if (typeInfo == typeid(PointLight)) {

		//登録済みか調べる。
		auto findIt = std::find(m_pointLights.begin(), m_pointLights.end(), light->GetLigData());
		if (findIt == m_pointLights.end()) {
			//新規登録。
			m_pointLights.push_back(reinterpret_cast<PointLigData*>(light->GetLigData()));

			int a = 0;
			std::list<PointLigData*>::iterator itr;
			itr = m_pointLights.begin();

			for (auto itr = m_pointLights.begin(); itr != m_pointLights.end(); ++itr) {

				m_ligData.pointLigData[a] = *(*itr);
				a++;
			}
			m_ligData.pointLightNum++;
		}
		else {
			//既に登録されている。
			MessageBoxA(nullptr, "既に登録されています", "エラー", MB_OK);
			return;
		}
	}
}

void LightManager::RemoveLight(LightBase* light)
{
	//クラスの名前を調べる
	const std::type_info& typeInfo = typeid(*light);
	
	//ディレクションライトだったら
	if (typeInfo == typeid(DirectionLight)) {

		//ライトを削除
		m_directionLights.erase(
			std::remove(m_directionLights.begin(), m_directionLights.end(), light->GetLigData()),
			m_directionLights.end()
		);

		int a = 0;

		//for (int i = 0; i < MAX_DIRECTION_LIGHT; i++) {
		//	m_ligData.m_directionLigData[i] = nullptr;
		//}

		for (auto itr = m_directionLights.begin(); itr != m_directionLights.end(); ++itr) {

			m_ligData.directionLigData[a] = *(*itr);
			a++;
		}
	}
	//ポイントライトだったら
	else if (typeInfo == typeid(PointLight)) {

		//ライトを削除
		m_pointLights.erase(
			std::remove(m_pointLights.begin(), m_pointLights.end(), light->GetLigData()),
			m_pointLights.end()
		);

		int a = 0;

		for (auto itr = m_pointLights.begin(); itr != m_pointLights.end(); ++itr) {

			m_ligData.pointLigData[a] = *(*itr);
			a++;
		}
		m_ligData.pointLightNum--;
	}
}
void LightManager::RemoveLightAll()
{
	//ライトをすべて削除する。
	m_directionLights.clear();
	m_pointLights.clear();
}
void LightManager::Update() {

	//カメラの座標を更新
	m_lightManager->m_ligData.eyePos = g_camera3D->GetPosition();
	//ライトカメラからプロジェクション行列を取得。
	m_lightManager->m_ligData.m_viewProj = Camera::GetLightCamera()->GetViewProjectionMatrix();

	for (auto itr = m_directionLights.begin(); itr != m_directionLights.end(); ++itr) {

		int a = 0;
		m_ligData.directionLigData[a] = *(*itr);
		a++;
	}

	//for (auto itr = m_pointLights.begin(); itr != m_pointLights.end(); itr++) {
	//
	//	int a = 0;
	//	m_ligData.pointLigData[a] = *(*itr);
	//	a++;
	//}
}