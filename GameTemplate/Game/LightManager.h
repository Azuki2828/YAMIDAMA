#pragma once

#include "stdafx.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "LightBase.h"

static const int c_maxDirectionLightNum = 4;	//ディレクションライトの上限数
static const int c_maxPointLightNum = 100;		//ポイントライトの上限数


//ライトのデータ
struct LigData {
	DirectionLigData directionLigData[c_maxDirectionLightNum];		//ディレクションライトのデータ
	PointLigData pointLigData[c_maxPointLightNum];					//ポイントライトのデータ
	Vector3 eyePos = Vector3::Zero;								//視点の座標
	int pointLightNum = 0;										//ポイントライトの総数
	Matrix m_viewProj;											//プロジェクション行列
};

//ライト管理クラス
class LightManager
{
public:
	/**
	 * @brief ライトマネージャーのインスタンスを作成する関数。
	*/
	static void CreateInstance() {

		m_lightManager = new LightManager;
	}
	
	/**
	 * @brief ライトマネージャーのインスタンスを取得する関数。
	 * @return インスタンス
	*/
	static LightManager* GetInstance() {

		return m_lightManager;
	}

	/**
	 * @brief ライトを追加する関数。
	 * @param light ライトのインスタンス
	*/
	void AddLight(LightBase* light);

	/**
	 * @brief ライトを削除する関数。
	 * @param light ライトのインスタンス
	*/
	void RemoveLight(LightBase* light);

	/**
	 * @brief 登録されているライトを全て削除する関数。
	*/
	void RemoveLightAll();
	/**
	 * @brief ライトのデータを取得する関数。
	 * @return ライトのデータ
	*/
	LigData* GetLigData() { return &m_ligData; }

	/**
	 * @brief 更新関数。
	*/
	void Update();
private:
	LigData m_ligData;									//ライトのデータ
	static LightManager* m_lightManager;				//ライトマネージャーのインスタンス
	std::list<DirectionLigData*> m_directionLights;		//ディレクションライトのデータ
	std::list<PointLigData*> m_pointLights;				//ポイントライトのデータ
};

