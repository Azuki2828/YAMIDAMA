#include "stdafx.h"
#include "FontEngine.h"

using namespace std;
using namespace DirectX;

namespace nsMyGame {

	namespace nsFont {

		CFontEngine::~CFontEngine()
		{
			if (m_srvDescriptorHeap != nullptr) {
				m_srvDescriptorHeap->Release();
			}
		}
		void CFontEngine::Init()
		{
			auto d3dDevice = g_graphicsEngine->GetD3DDevice();

			//ディスクリプタヒープを作成。
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

			srvHeapDesc.NumDescriptors = 1;
			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvDescriptorHeap));

			ResourceUploadBatch re(d3dDevice);
			re.Begin();
			//SpriteBatchのパイプラインステートを作成する。
			RenderTargetState renderTargetState;
			renderTargetState.rtvFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			renderTargetState.numRenderTargets = 1;
			renderTargetState.dsvFormat = DXGI_FORMAT_D32_FLOAT;
			renderTargetState.sampleMask = UINT_MAX;
			renderTargetState.sampleDesc.Count = 1;

			SpriteBatchPipelineStateDescription sprBatchDesc(renderTargetState);

			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth();
			viewport.Height = g_graphicsEngine->GetFrameBufferHeight();
			//Spriteバッチを作成。
			m_spriteBatch = make_unique<SpriteBatch>(
				d3dDevice,
				re,
				sprBatchDesc,
				&viewport);
			//SpriteFontを作成。
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
			m_spriteFont = make_unique<SpriteFont>(
				d3dDevice,
				re,
				L"Assets/font/myfile.spritefont",
				cpuHandle,
				gpuHandle);

			re.End(g_graphicsEngine->GetCommandQueue());
		}

		void CFontEngine::BeginDraw(CRenderContext& rc)
		{
			auto commandList = g_graphicsEngine->GetCommandList();
			auto d3dDevice = g_graphicsEngine->GetD3DDevice();
			m_spriteBatch->Begin(
				commandList,
				SpriteSortMode_Deferred,
				g_matIdentity
			);
			commandList->SetDescriptorHeaps(1, &m_srvDescriptorHeap);
		}
		void CFontEngine::EndDraw(CRenderContext& rc)
		{
			m_spriteBatch->End();
		}

		void CFontEngine::Draw(
			const wchar_t* text,
			const CVector2& position,
			const CVector4& color,
			float rotation,
			float scale,
			CVector2 pivot
		)
		{
			m_spriteFont->DrawString(
				m_spriteBatch.get(),
				text,
				position.vec,
				color,
				rotation,
				DirectX::XMFLOAT2(pivot.x, pivot.y),
				scale
			);
		}
	}
}