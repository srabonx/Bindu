#include "D3DPipelineStateManager.h"

#include "D3DDeviceManager.h"
#include "D3DUtillity.h"
#include "D3DShader.h"

#include "../../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	void D3DPipelineStateManager::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager)
	{
		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		m_parentDeviceManager = deviceManager;
	}

	ID3D12PipelineState* D3DPipelineStateManager::CreatePipelineState(const std::string& name, const PipelineStateDesc& pipelineState)
	{
		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();
		
		if (!pipelineState.RootSignature)
			THROW_EXCEPTION(3, "Invalid root signature");

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout.NumElements = pipelineState.InputElementDescs.size(); // Input layout for vertex data
		psoDesc.InputLayout.pInputElementDescs = pipelineState.InputElementDescs.data();
		psoDesc.pRootSignature = pipelineState.RootSignature;
		psoDesc.VS = pipelineState.VertexShader ? pipelineState.VertexShader->GetShaderByteCode() : D3D12_SHADER_BYTECODE(); // Vertex shader
		psoDesc.PS = pipelineState.PixelShader ? pipelineState.PixelShader->GetShaderByteCode() : D3D12_SHADER_BYTECODE();  // Pixel shader
		psoDesc.GS = pipelineState.GeometryShader ? pipelineState.GeometryShader->GetShaderByteCode() : D3D12_SHADER_BYTECODE(); // Geometry shader
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = pipelineState.CullMode; // Set culling mode
		psoDesc.RasterizerState.FillMode = pipelineState.FillMode;
		psoDesc.RasterizerState.FrontCounterClockwise = pipelineState.FrontFaceCounterClockwise;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.BlendState.RenderTarget[0] = pipelineState.RTBlendDesc.BlendEnable ? pipelineState.RTBlendDesc : psoDesc.BlendState.RenderTarget[0];
		psoDesc.DepthStencilState = pipelineState.DepthStencilDesc;//CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = pipelineState.PrimitiveTopologyType;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // Render target format
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // Depth-stencil format
		psoDesc.SampleDesc.Count = 1;

		ComPtr<ID3D12PipelineState>	pso{ nullptr };

		DXThrowIfFailed(
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pso.ReleaseAndGetAddressOf()))
		);

		m_pipelineStateObjects.emplace(name, std::make_pair(pipelineState, pso));

		return pso.Get();
	}

	ID3D12RootSignature* D3DPipelineStateManager::CreateRootSignature(const std::string& name, const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc)
	{
		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();

		ComPtr<ID3D12RootSignature> rootSig{ nullptr };
		ComPtr<ID3DBlob> rootSigBlob{ nullptr };
		ComPtr<ID3DBlob> errorBlob{ nullptr };

		DXThrowIfFailed(
			D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
				rootSigBlob.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf()));

		if (errorBlob)
			THROW_EXCEPTION(3, reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		DXThrowIfFailed(
			d3dDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
				rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSig.ReleaseAndGetAddressOf())));

		m_rootSignatures.emplace(name, rootSig);

		return rootSig.Get();
	}

	ID3D12PipelineState* D3DPipelineStateManager::GetPipelineState(const std::string& name) const
	{
		return m_pipelineStateObjects.at(name).second.Get();
	}

	ID3D12RootSignature* D3DPipelineStateManager::GetRootSignature(const std::string& name) const
	{
		return m_rootSignatures.at(name).Get();
	}
}
