#include "stdafx.h"
#include "BackGroundObject.h"

namespace nsMyGame {

	namespace {

		constexpr float c_objFriction = 10.0f;		//摩擦
	}

	void CBackGroundObject::InitModel(const char* fileName,
		const CVector3& position, 
		const CQuaternion& rotation, 
		const CVector3& scale,
		const bool isCollision
	)
	{
		//モデルを生成。
		m_modelRender = NewGO<CModelRender>(enPriority_Zeroth);

		//ファイルパスを指定。
		m_modelRender->SetFilePathTkm(fileName);

		//シャドウレシーバーフラグを設定。
		m_modelRender->SetShadowReceiverFlag(true);

		//初期化。
		m_modelRender->Init();

		//各パラメータを設定。
		m_modelRender->SetPosition(position);	//座標
		m_modelRender->SetRotation(rotation);	//回転
		m_modelRender->SetScale(scale);			//拡大

		//パラメータを更新。
		m_modelRender->UpdateWorldMatrix();

		//コリジョンを生成する？
		if (isCollision) {

			//モデルを無視（描画しないように）する？
			if (m_isIgnoreObject) {

				m_modelRender->SetCollisionModelFlag(true);
			}

			//コリジョンを生成。
			m_physicsStaticObject.CreateFromModel(
				*m_modelRender->GetModel(),
				m_modelRender->GetModel()->GetWorldMatrix()
			);

			//摩擦を設定。
			m_physicsStaticObject.SetFriction(c_objFriction);
		}
	}
}
