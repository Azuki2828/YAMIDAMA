#pragma once

#include "Indexbuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

class Texture;

//スプライトに設定できる最大テクスチャ数。
const int MAX_TEXTURE = 16;	
//拡張SRVが設定されるレジスタの開始番号。
const int EXPAND_SRV_REG__START_NO = 10;
const int EXPAND_SRV_REG__START_NO2 = 11;

class IShaderResource;

/// <summary>
/// アルファブレンディングモード
/// </summary>
enum AlphaBlendMode {
	AlphaBlendMode_None,	//アルファブレンディングなし(上書き)。
	AlphaBlendMode_Trans,	//半透明合成
	AlphaBlendMode_Add,		//加算合成
};
/// <summary>
/// スプライトの初期化データ。
/// </summary>
struct SpriteInitData {
	const char* m_ddsFilePath[MAX_TEXTURE]= {nullptr};		//DDSファイルのファイルパス。
	Texture* m_textures[MAX_TEXTURE] = { nullptr };			//使用するテクスチャ。DDSファイルのパスが指定されている場合は、このパラメータは無視されます。
	const char* m_vsEntryPointFunc = "VSMain";				//頂点シェーダーのエントリーポイント。
	const char* m_psEntryPoinFunc = "PSMain";				//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;						//.fxファイルのファイルパス。
	UINT m_width = 0;										//スプライトの幅。
	UINT m_height = 0;										//スプライトの高さ。
	void* m_expandConstantBuffer = nullptr;					//ユーザー拡張の定数バッファ
	int m_expandConstantBufferSize = 0;						//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
	IShaderResource* m_expandShaderResoruceView2 = nullptr;	//ユーザー拡張のシェーダーリソース。
	AlphaBlendMode m_alphaBlendMode = AlphaBlendMode_None;	//アルファブレンディングモード。
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
};
/// <summary>
/// スプライトクラス。
/// </summary>
class CSprite  {
public:
	static const CVector2	DEFAULT_PIVOT;					//!<ピボット。
	virtual ~CSprite();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	void Init(const SpriteInitData& initData);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pivot">
	/// ピボット
	/// 0.5, 0.5で画像の中心が基点。
	/// 0.0, 0.0で画像の左下。
	/// 1.0, 1.0で画像の右上。
	/// UnityのuGUIに準拠。
	/// </param>
	void Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot = DEFAULT_PIVOT);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト/param>
	void Draw(CRenderContext& renderContext);
	/// <summary>
	/// 初期化されているか判定。
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}
private:
	/// <summary>
	/// テクスチャを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitTextures(const SpriteInitData& initData);
	/// <summary>
	/// シェーダーを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitShader( const SpriteInitData& initData );
	/// <summary>
	/// ディスクリプタヒープを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitDescriptorHeap(const SpriteInitData& initData);
	/// <summary>
	/// 頂点バッファとインデックスバッファを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitVertexBufferAndIndexBuffer(const SpriteInitData& initData);
	/// <summary>
	/// パイプラインステートを初期化する。
	/// </summary>
	void InitPipelineState(const SpriteInitData& initData);
	/// <summary>
	/// 定数バッファを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitConstantBuffer(const SpriteInitData& initData);

public:
	void SetPosition(const CVector3& pos) {
		m_position = pos;
	}
	void SetRotation(const CQuaternion& rot) {
		m_rotation = rot;
	}
	void SetScale(const CVector3& sca) {
		m_scale = sca;
	}
	void SetMulColor(const CVector4& mulCol) {

		m_mulColor = mulCol;
	}
private:
	IndexBuffer m_indexBuffer;			//インデックスバッファ。
	VertexBuffer m_vertexBuffer;		//頂点バッファ。
	int m_numTexture = 0;				//テクスチャの枚数。
	Texture m_textures[MAX_TEXTURE];	//テクスチャ。
	Texture* m_textureExternal[MAX_TEXTURE] = {nullptr};	//外部から指定されたテクスチャ
	CVector3 m_position;				//座標。
	CVector3 m_scale;				//拡大率。
	CVector2 m_size;						//サイズ。
	CQuaternion m_rotation ;			//回転。
	CVector4 m_mulColor = { 1.0f,1.0f,1.0f,1.0f };		//乗算カラー。
	CMatrix m_world;					//ワールド行列。

	struct LocalConstantBuffer {
		CMatrix mvp;
		CVector4 mulColor;
		CVector4 screenParam;
	};
	LocalConstantBuffer m_constantBufferCPU;	//CPU側の定数バッファ。
	ConstantBuffer		m_constantBufferGPU;	//GPU側の定数バッファ。
	ConstantBuffer		m_userExpandConstantBufferGPU;	//ユーザー拡張の定数バッファ(GPU側)
	void* m_userExpandConstantBufferCPU = nullptr;		//ユーザー拡張の定数バッファ(CPU側)
	DescriptorHeap		m_descriptorHeap;		//ディスクリプタヒープ。
	RootSignature		m_rootSignature;		//ルートシグネチャ。
	PipelineState		m_pipelineState;		//パイプラインステート。
	Shader				m_vs;					//頂点シェーダー。
	Shader				m_ps;					//ピクセルシェーダー。
	bool				m_isInited = false;		//初期化済み？
};