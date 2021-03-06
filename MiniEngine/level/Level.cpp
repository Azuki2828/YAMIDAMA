#include "stdafx.h"
#include "Level.h"

namespace nsMyGame {


	void Level::Init(
		const char* filePath,
		std::function<bool(LevelObjectData& objData)> hookFunc
	)
	{
		//スケルトンをロードする。
		//Skeleton skeleton;
		m_tklFile.Load(filePath);

		for (auto i = 1; i < m_tklFile.GetBoneNum(); i++) {
			auto bone = m_tklFile.GetBonePtr(i);

			if (bone->GetParentBoneNo() == 0) {
				LevelObjectData objData;
				CVector3 scale;
				bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);

				//3dsMaxとは軸が違うので、補正を入れる。
				auto t = objData.position.y;
				objData.position.y = objData.position.z;
				objData.position.z = t;



				t = objData.rotation.y;
				objData.rotation.y = -objData.rotation.z;
				objData.rotation.z = -t;
				objData.rotation.x *= -1.0f;


				objData.name = bone->GetName();

				std::swap(objData.scale.y, objData.scale.z);


				auto isHook = false;
				if (hookFunc != nullptr) {

					//hook関数が指定されているのでhook関数を呼び出す。
					isHook = hookFunc(objData);
				}
				if (isHook == false) {
					//マップチップレンダラーを作成する。

					m_mapChipPtrs.push_back(std::make_unique<MapChip>(objData));
				}
			}
		}


		////構築構築。
		////0番目はルートオブジェクトなので飛ばす。
		//for (auto i = 1; i < skeleton.GetNumBones(); i++) {
		//	//骨を取得。
		//	auto bone = skeleton.GetBone(i);
		//	if (bone->GetParentBoneNo() == 0) {	//親がルートの場合だけマップチップを生成する。
		//		LevelObjectData objData;
		//		Vector3 scale;
		//		bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);
		//		//3dsMaxとは軸が違うので、補正を入れる。
		//		auto t = objData.position.y;
		//		objData.position.y = objData.position.z;
		//		objData.position.z = -t;
		//
		//		t = objData.rotation.y;
		//		objData.rotation.y = objData.rotation.z;
		//		objData.rotation.z = -t;
		//		objData.name = bone->GetName();
		//
		//		std::swap(objData.scale.y, objData.scale.z);
		//
		//		auto isHook = false;
		//		if (hookFunc != nullptr) {
		//			//hook関数が指定されているのでhook関数を呼び出す。
		//			isHook = hookFunc(objData);
		//		}
		//		if (isHook == false) {
		//			//マップチップレンダラーを作成する。
		//
		//			m_mapChipPtrs.push_back(std::make_unique<MapChip>(objData));
		//			
		//		}
		//	}
		//}
	}

	void Level::Draw(CRenderContext& rc) {

		for (int i = 0; i < m_mapChipPtrs.size(); i++) {
			m_mapChipPtrs[i]->Draw(rc);
		}
	}
}
