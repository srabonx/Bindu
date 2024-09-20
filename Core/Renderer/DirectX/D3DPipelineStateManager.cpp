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

	void D3DPipelineStateManager::AddPipelineState(const std::string& name, const D3DShader& vertexShader,
		const D3DShader& pixelShader, const D3D12_INPUT_LAYOUT_DESC& inputLayout, const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc)
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

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = inputLayout; // Input layout for vertex data
		psoDesc.pRootSignature = rootSig.Get();
		psoDesc.VS = vertexShader.GetShaderByteCode(); // Vertex shader
		psoDesc.PS = pixelShader.GetShaderByteCode();  // Pixel shader
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // Set culling mode
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // Render target format
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // Depth-stencil format
		psoDesc.SampleDesc.Count = 1;

		ComPtr<ID3D12PipelineState>	pso{ nullptr };

		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pso.ReleaseAndGetAddressOf()));

		m_pipelineStateObjects.emplace(name, pso);

		m_rootSignatures.emplace(name, rootSig);

	}

	ID3D12PipelineState* D3DPipelineStateManager::GetPipelineState(const std::string& name) const
	{
		return m_pipelineStateObjects.at(name).Get();
	}

	ID3D12RootSignature* D3DPipelineStateManager::GetRootSignature(const std::string& name) const
	{
		return m_rootSignatures.at(name).Get();
	}
}
