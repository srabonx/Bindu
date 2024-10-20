#include "DirectXRenderer.h"

#include "D3DUtillity.h"
#include "d3dx12.h"
#include "DirectXTexture.h"
#include "DDSTextureLoader.h"
#include "DirectXRenderTexture.h"
#include "DirectXShader.h"
#include "GpuDescriptorHeap.h"
#include "../../../Debug/Assert.h"
#include "../../../Debug/Profiler.h"

namespace BINDU
{
	void DirectXRenderer::Initialize()
	{
		m_deviceManager = CreateRef<D3DDeviceManager>();
		m_deviceManager->Initialize();

		m_commandContext = CreateRef<D3DCommandContext>();
		m_commandContext->Initialize(m_deviceManager);

		m_commandList = m_commandContext->GetCommandList();
	}

	void DirectXRenderer::Close()
	{
		m_deviceManager->Close();
	}

	Ref<Texture> DirectXRenderer::CreateTexture(const std::string& path)
	{
		BINDU_PROFILE_FUNCTION();

		ComPtr<ID3D12Resource> d3dTexture{nullptr};
		ComPtr<ID3D12Resource> d3dTextureUploader{nullptr};

		auto d3dDevice = m_deviceManager->GetD3DDevice();
		auto fileName = StringToWstring(path);

		DXThrowIfFailed(
			DirectX::CreateDDSTextureFromFile12(d3dDevice, m_commandList.Get(), fileName.c_str(),
				d3dTexture, d3dTextureUploader));

		auto desc = d3dTexture->GetDesc();

		TextureSpecification specification = { static_cast<std::uint32_t>(desc.Width), desc.Height, TextureFormat::RGBA8_UNORM };

		auto texture = CreateRef<DirectXTexture>(specification, m_commandList);

		texture->m_d3dResource = std::move(d3dTexture);
		texture->m_d3dUploader = std::move(d3dTextureUploader);

		texture->m_allocation = m_deviceManager->GetCbvSrvUavGpuHeap()->Allocate(1);

		texture->m_isLoaded = true;

		return texture;
	}

	Ref<Texture> DirectXRenderer::CreateDepthStencilTexture(const TextureSpecification& specification, bool isShaderVisible)
	{

		BINDU_PROFILE_FUNCTION();

		auto format = D3DUtility::GetDXGIFormatFromTextureFormat(specification.Format);

		D3D12_CLEAR_VALUE clearVal = {};
		clearVal.Format = format;
		clearVal.DepthStencil.Depth = 1.0f;
		clearVal.DepthStencil.Stencil = 0;

		auto resource = m_deviceManager->CreateCommitedTex2D(specification.Width, specification.Height, format, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES, &clearVal);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.Format = format;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		auto texture = CreateRef<DirectXTexture>(specification, m_commandList);

		texture->m_allocation = m_deviceManager->CreateDepthStencilView(resource.Get(), dsvDesc);

		texture->m_d3dResource = std::move(resource);

		return texture;
	}

	Ref<Texture> DirectXRenderer::CreateRenderTexture(const Texture* depthStencilTexture, const TextureSpecification& specification, bool isShaderVisible)
	{

		BINDU_PROFILE_FUNCTION();

		auto format = D3DUtility::GetDXGIFormatFromTextureFormat(specification.Format);

		D3D12_CLEAR_VALUE clearVal = {};
		clearVal.Format = format;
		clearVal.Color[0] = 1.0f;
		clearVal.Color[1] = 0.0f;
		clearVal.Color[2] = 0.0f;
		clearVal.Color[3] = 1.0f;

		auto resource = m_deviceManager->CreateCommitedTex2D(specification.Width, specification.Height, format, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES);

		return CreateRenderTexture(resource, depthStencilTexture, specification, isShaderVisible);
	}

	Ref<Shader> DirectXRenderer::CreateShader(const ShaderSpecification& specification)
	{

		BINDU_CORE_ASSERT(!specification.Filepath.empty(), "Shader Filepath cannot be empty!");

		BINDU_PROFILE_FUNCTION();

		auto shader = CreateRef<DirectXShader>(m_commandList);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

		CD3DX12_ROOT_PARAMETER slotRootParam[specification.ShaderParameters.size()];

		for (int i = 0; i < specification.ShaderParameters.size(); i++)
		{
			auto shaderParameter = specification.ShaderParameters[i];
			switch (shaderParameter.Type)
			{
			case ShaderParameterType::TEXTURE:
				CD3DX12_DESCRIPTOR_RANGE srvRange;
				srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, shaderParameter.Slot);
				slotRootParam[i].InitAsDescriptorTable(1, &srvRange);
				shader->m_texSlotToRootParam[shaderParameter.Slot] = i;
				break;
			case ShaderParameterType::UNIFORM_BUFFER:
				slotRootParam[i].InitAsConstantBufferView(shaderParameter.Slot);
				shader->m_cbSlotToRootParam[shaderParameter.Slot] = i;
				break;
			}
		}

		auto staticSamplers = D3DUtility::GetStaticSamplers();

		CD3DX12_ROOT_SIGNATURE_DESC rsd(specification.ShaderParameters.size(), slotRootParam,
			staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		shader->m_rootSig = m_deviceManager->CreateRootSignature(rsd);



		if (!specification.VertexShader.empty())
		{
			shader->m_vsBlob = D3DUtility::CompileShader(specification.Filepath, nullptr, specification.VertexShader, "vs_5_0");

			shader->m_vsByteCode.BytecodeLength = shader->m_vsBlob->GetBufferSize();
			shader->m_vsByteCode.pShaderBytecode = shader->m_vsBlob->GetBufferPointer();

			psoDesc.VS = shader->m_vsByteCode;
		}
		if (!specification.PixelShader.empty())
		{
			shader->m_psBlob = D3DUtility::CompileShader(specification.Filepath, nullptr, specification.PixelShader, "ps_5_0");

			shader->m_psByteCode.BytecodeLength = shader->m_psBlob->GetBufferSize();
			shader->m_psByteCode.pShaderBytecode = shader->m_psBlob->GetBufferPointer();

			psoDesc.PS = shader->m_psByteCode;
		}
		if (!specification.GeometryShader.empty())
		{
			shader->m_gsBlob = D3DUtility::CompileShader(specification.Filepath, nullptr, specification.GeometryShader, "gs_5_0");

			shader->m_gsByteCode.BytecodeLength = shader->m_gsBlob->GetBufferSize();
			shader->m_gsByteCode.pShaderBytecode = shader->m_gsBlob->GetBufferPointer();

			psoDesc.GS = shader->m_gsByteCode;
		}
		if (!specification.ComputeShader.empty())
		{
			shader->m_csBlob = D3DUtility::CompileShader(specification.Filepath, nullptr, specification.ComputeShader, "cs_5_0");

			shader->m_csByteCode.BytecodeLength = shader->m_csBlob->GetBufferSize();
			shader->m_csByteCode.pShaderBytecode = shader->m_csBlob->GetBufferPointer();

			D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc = {};

			computePsoDesc.pRootSignature = shader->m_rootSig.Get();
			computePsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			computePsoDesc.CS = shader->m_csByteCode;

			shader->m_computePSO = m_deviceManager->CreateComputePipelineState(computePsoDesc);
		}


		

		std::uint32_t index{ 0 };
		for(auto& e : specification.ShaderInputElements)
		{
			auto dxgiFormat = D3DUtility::GetDXGIFormatFromDataFormat(e.Format);
			auto dataByteOffset = index * e.DataByteSize;
			D3D12_INPUT_ELEMENT_DESC eld = { e.SemanticName.c_str(), 0, dxgiFormat, 0, dataByteOffset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			shader->m_inputElementDesc.emplace_back(eld);
			index++;
		}

		psoDesc.InputLayout.NumElements = shader->m_inputElementDesc.size(); // Input layout for vertex data
		psoDesc.InputLayout.pInputElementDescs = shader->m_inputElementDesc.data();
		psoDesc.pRootSignature = shader->m_rootSig.Get();
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // Set culling mode
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // Render target format
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // Depth-stencil format
		psoDesc.SampleDesc.Count = 1;

		shader->m_graphicsPSO = m_deviceManager->CreateGraphicsPipelineState(psoDesc);
		return shader;
	}

	const Ref<D3DCommandContext>& DirectXRenderer::GetCommandContext() const
	{
		return m_commandContext;
	}

	Ref<Texture> DirectXRenderer::CreateRenderTexture(const ComPtr<ID3D12Resource>& d3dResource,
		const Texture* depthStencilTexture, const TextureSpecification& specification, bool isShaderVisible)
	{
		DXGI_FORMAT format = D3DUtility::GetDXGIFormatFromTextureFormat(specification.Format);

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		auto dxDsvTexture = dynamic_cast<const DirectXTexture*>(depthStencilTexture);

		BINDU_CORE_ASSERT(dxDsvTexture, "Texture is not created as DepthStencil Texture");

		auto texture = CreateRef<DirectXRenderTexture>(specification, m_commandList, dxDsvTexture->m_allocation.GetCpuHandle());

		texture->m_rtvAllocation = m_deviceManager->CreateRenderTargetView(d3dResource.Get(), rtvDesc);

		if (isShaderVisible)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;

			texture->m_srvAllocation = m_deviceManager->CreateShaderResourceView(d3dResource.Get(), srvDesc);

			texture->m_isShaderVisible = true;
		}

		texture->m_d3dResource = std::move(d3dResource);

		return texture;
	}
}
