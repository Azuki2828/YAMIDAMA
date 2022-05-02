#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"
#include "Material.h"

#include "../../GameTemplate/Game/BackGroundObject.h"


namespace nsMyGame {

	struct LevelObjectData;

	MapChip::MapChip(const LevelObjectData& objData) {

		char objName[256];
		wcstombs(objName, objData.name, 256);

		char* p;
		p = strstr(objName, "Coll");

		if (p == nullptr) {

			//ファイルパスを決定。
			char filePathtkm[256];
			sprintf(filePathtkm, "Assets/modelData/backGround/%s.tkm", objName);

			//モデルを初期化。
			CBackGroundObject* model = NewGO<CBackGroundObject>(enPriority_Zeroth);
			model->InitModel(
				static_cast<const char*>(filePathtkm),
				objData.position,
				objData.rotation,
				objData.scale,
				true
			);

		}
		else {

			//ファイルパスを決定。
			char filePathtkm[256];
			strcpy(objName, (p + 4));
			sprintf(filePathtkm, "Assets/modelData/backGround/%s.tkm", objName);

			//モデルを初期化。
			CBackGroundObject* model = NewGO<CBackGroundObject>(enPriority_Zeroth);
			model->InitModel(
				static_cast<const char*>(filePathtkm),
				objData.position,
				objData.rotation,
				objData.scale,
				false
			);

			//ファイルパスを決定。
			char filePathtkm2[256];
			sprintf(filePathtkm2, "Assets/modelData/backGround/Coll%s.tkm", p);

			//モデルを初期化。
			CBackGroundObject* collModel = NewGO<CBackGroundObject>(enPriority_Zeroth);

			collModel->IgnoreObj();

			collModel->InitModel(
				static_cast<const char*>(filePathtkm2),
				objData.position,
				objData.rotation,
				objData.scale,
				true
			);
		}
	}

	MapChip::~MapChip() {
		if (m_modelRender != nullptr) {
			DeleteGO(m_modelRender);
		}
		if (m_collisionModelRender != nullptr) {
			DeleteGO(m_collisionModelRender);
		}
	}

	void MapChip::Draw(CRenderContext& rc) {
		//Draw(rc,
		//	m_skinModelRender->GetModel()->GetWorldMatrix(),
		//	g_camera3D->GetViewMatrix(),
		//	g_camera3D->GetProjectionMatrix());

	}

	void MapChip::Draw(CRenderContext& rc,
		const CMatrix& mWorld,
		const CMatrix& mView,
		const CMatrix& mProj) {
		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファを更新する。
		MConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;

		m_constantBuffer.CopyToVRAM(&cb);

		//if (m_expandData) {
		//	m_expandConstantBuffer.CopyToVRAM(m_expandData);
		//}
		//if (m_boneMatricesStructureBuffer.IsInited()) {
		//	//ボーン行列を更新する。
		//	m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
		//}
		int descriptorHeapNo = 0;
		for (auto& mesh : m_meshs) {
			//1. 頂点バッファを設定。
			rc.SetVertexBuffer(mesh->m_vertexBuffer);
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//このマテリアルが貼られているメッシュの描画開始。
				mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
				//2. ディスクリプタヒープを設定。
				rc.SetDescriptorHeap(m_descriptorHeap.at(descriptorHeapNo));
				//3. インデックスバッファを設定。
				auto& ib = mesh->m_indexBufferArray[matNo];
				rc.SetIndexBuffer(*ib);

				//4. ドローコールを実行。
				rc.DrawIndexed(ib->GetCount());
				descriptorHeapNo++;
			}
		}
	}
}