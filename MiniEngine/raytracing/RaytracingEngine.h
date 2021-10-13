#pragma once

#include "DescriptorHeap.h"
#include "RaytracingInstance.h"
#include "TLASBuffer.h"
#include "BLASBuffer.h"
#include "RaytracingPSO.h"
#include "RaytracingShaderTable.h"
#include "RaytracingDescriptorHeaps.h"

class CModel;

namespace raytracing {
	

	class Engine
	{
	public:
		/// <summary>
		/// レイトレーシングをディスパッチ。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Dispatch(CRenderContext& rc);
		/// <summary>
		/// ジオメトリを登録。
		/// </summary>
		/// <param name="model">モデル</param>
		void RegistGeometry(CModel& model)
		{
			//レイトレワールドにジオメトリを登録。
			m_world.RegistGeometry(model);
		}
		/// <summary>
		/// ジオメトリの登録を確定。
		/// </summary>
		void CommitRegistGeometry(CRenderContext& rc);
	private:
		/// <summary>
		/// シェーダーテーブルを作成。
		/// </summary>
		/// <param name="rc"></param>
		void CreateShaderTable(CRenderContext& rc);
		
		/// <summary>
		/// シェーダーリソースを作成。
		/// </summary>
		void CreateShaderResources();

	private:
		/// <summary>
		/// カメラ
		/// </summary>
		struct Camera {
			CMatrix mRot;	//回転行列
			CVector3 pos;	//視点。
			float aspect;	//アスペクト比。
			float fFar;		//遠平面。
			float fNear;	//近平面。
		};
		ConstantBuffer m_rayGenerationCB;			//レイジェネレーションの定数バッファ。
		World m_world;								//レイトレワールド。
		PSO m_pipelineStateObject;					//パイプラインステートオブジェクト
		ShaderTable m_shaderTable;					//シェーダーテーブル。
		DescriptorHeaps m_descriptorHeaps;			//レイトレで使用するディスクリプタヒープの管理者。
		GPUBuffer m_outputResource;					//レイトレースの結果の出力先。
	};
}//namespace raytracing