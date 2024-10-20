#include "D3DDeviceManager.h"

#include "CpuDescriptorHeap.h"
#include "D3DUtillity.h"
#include "GpuDescriptorHeap.h"
#include <dxgidebug.h>

#include "DDSTextureLoader.h"
#include "../../../Debug/Assert.h"
#include "../../Resources/Resource.h"

namespace BINDU
{
	void D3DDeviceManager::Initialize()
	{

		InitD3D();

		// Create CommandQueue
		D3D12_COMMAND_QUEUE_DESC qDesc = {};
		qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		qDesc.NodeMask = 0;
		qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		DXThrowIfFailed(
			m_d3dDevice->CreateCommandQueue(&qDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf())));

		// Create the CPU and GPU descriptor heaps
		m_rtvCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_dsvCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_cbvSrvUavCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_samplerCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		m_cbvSrvUavGpuHeap = std::make_shared<GpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_samplerGpuHeap = std::make_shared<GpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		m_rtvCpuHeap->Initialize();
		m_dsvCpuHeap->Initialize();
		m_cbvSrvUavCpuHeap->Initialize();
		m_samplerCpuHeap->Initialize();

		m_cbvSrvUavGpuHeap->Initialize();
		m_samplerGpuHeap->Initialize();
	}

	void D3DDeviceManager::Close()
	{
#if defined(DEBUG) || defined(_DEBUG)
		ComPtr<IDXGIDebug1> pdxgiDebug{ nullptr };
		DXThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(pdxgiDebug.ReleaseAndGetAddressOf())));
		DXThrowIfFailed(pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL));
#endif
	}

	ComPtr<ID3D12Resource> D3DDeviceManager::CreateCommitedTex2D(std::uint64_t width, std::uint32_t height,
		DXGI_FORMAT format, D3D12_HEAP_FLAGS heapFlag, const D3D12_CLEAR_VALUE* optimizedClearVal) const
	{

		ComPtr<ID3D12Resource> resource{ nullptr };

		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		auto rsrcDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);

		DXThrowIfFailed(
			m_d3dDevice->CreateCommittedResource(&heapProp, heapFlag, &rsrcDesc,
				D3D12_RESOURCE_STATE_COMMON, optimizedClearVal, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf())));

		return resource;
	}

	ComPtr<ID3D12Resource> D3DDeviceManager::CreateCommitedBuffer(std::uint64_t width, D3D12_HEAP_TYPE heapType, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_STATES initialState) const
	{
		ComPtr<ID3D12Resource> resource;

		auto heapProp = CD3DX12_HEAP_PROPERTIES(heapType);

		auto rsDesc = CD3DX12_RESOURCE_DESC::Buffer(width);

		DXThrowIfFailed(
			m_d3dDevice->CreateCommittedResource(&heapProp, heapFlags, &rsDesc, initialState,
				nullptr, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf())));

		return resource;
	}

	ComPtr<ID3D12PipelineState> D3DDeviceManager::CreateGraphicsPipelineState(
		const D3D12_GRAPHICS_PIPELINE_STATE_DESC& psd) const
	{
		ComPtr<ID3D12PipelineState> pipelineState{ nullptr };

		DXThrowIfFailed(
			m_d3dDevice->CreateGraphicsPipelineState(&psd, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())));

		return pipelineState;
	}

	ComPtr<ID3D12PipelineState> D3DDeviceManager::CreateComputePipelineState(
		const D3D12_COMPUTE_PIPELINE_STATE_DESC& psd) const
	{
		ComPtr<ID3D12PipelineState> pipelineState{ nullptr };

		DXThrowIfFailed(
			m_d3dDevice->CreateComputePipelineState(&psd, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())));

		return pipelineState;
	}

	ComPtr<ID3D12RootSignature> D3DDeviceManager::CreateRootSignature(
		const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc) const
	{
		ComPtr<ID3D12RootSignature> rootSig{ nullptr };

		ComPtr<ID3DBlob> rootSigBlob{ nullptr };
		ComPtr<ID3DBlob> errorBlob{ nullptr };

		DXThrowIfFailed(
			D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, rootSigBlob.GetAddressOf(), errorBlob.GetAddressOf()));

		if(errorBlob)
		{
			std::string errorStr = "Root Signature creation failed. Error : " + std::string(static_cast<char*>(errorBlob->GetBufferPointer()));
			BINDU_CORE_ASSERT(false, errorStr.c_str());
		}

		DXThrowIfFailed(
			m_d3dDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
				rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSig.ReleaseAndGetAddressOf())));

		return rootSig;
	}

	DescriptorHeapAllocation&& D3DDeviceManager::CreateDepthStencilView(
		ID3D12Resource* depthStencilResource, const D3D12_DEPTH_STENCIL_VIEW_DESC& depthStencilDesc) const
	{
		auto allocation = m_dsvCpuHeap->Allocate(1);

		m_d3dDevice->CreateDepthStencilView(depthStencilResource, &depthStencilDesc, allocation.GetCpuHandle());

		return std::move(allocation);
	}


	DescriptorHeapAllocation&& D3DDeviceManager::CreateRenderTargetView(ID3D12Resource* renderTargetResource,
		const D3D12_RENDER_TARGET_VIEW_DESC& rtvDesc) const
	{
		auto allocation = m_rtvCpuHeap->Allocate(1);

		m_d3dDevice->CreateRenderTargetView(renderTargetResource, &rtvDesc, allocation.GetCpuHandle());

		return std::move(allocation);
	}

	DescriptorHeapAllocation&& D3DDeviceManager::CreateShaderResourceView(ID3D12Resource* srvResource,
		const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc) const
	{
		auto allocation = m_cbvSrvUavGpuHeap->Allocate(1);

		m_d3dDevice->CreateShaderResourceView(srvResource, &srvDesc, allocation.GetCpuHandle());

		return std::move(allocation);
	}

	DirectXConstantBuffer&& D3DDeviceManager::CreateConstantBuffer(std::uint32_t elementCount) const
	{
		DirectXConstantBuffer buffer(elementCount);

		auto width = buffer.m_elementByteSize * elementCount;

		buffer.m_resource = CreateCommitedBuffer(width, D3D12_HEAP_TYPE_UPLOAD, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

		return std::move(buffer);
	}

	ID3D12Device* D3DDeviceManager::GetD3DDevice() const
	{
		return m_d3dDevice.Get();
	}

	ID3D12CommandQueue* D3DDeviceManager::GetCommandQueue() const
	{
		return m_commandQueue.Get();
	}

	CpuDescriptorHeap* D3DDeviceManager::GetRtvCpuHeap() const
	{
		return m_rtvCpuHeap.get();
	}

	CpuDescriptorHeap* D3DDeviceManager::GetDsvCpuHeap() const
	{
		return m_dsvCpuHeap.get();
	}

	CpuDescriptorHeap* D3DDeviceManager::GetCbvSrvUavCpuHeap() const
	{
		return m_cbvSrvUavCpuHeap.get();
	}

	CpuDescriptorHeap* D3DDeviceManager::GetSamplerCpuHeap() const
	{
		return m_samplerCpuHeap.get();
	}

	GpuDescriptorHeap* D3DDeviceManager::GetCbvSrvUavGpuHeap() const
	{
		return m_cbvSrvUavGpuHeap.get();
	}

	GpuDescriptorHeap* D3DDeviceManager::GetSamplerGpuHeap() const
	{
		return m_samplerGpuHeap.get();
	}

	void D3DDeviceManager::InitD3D()
	{
#if defined(DEBUG) || defined(_DEBUG)
		// Enabling the debug layer
		ComPtr<ID3D12Debug6>	pd3dDebug{ nullptr };

		DXThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(pd3dDebug.ReleaseAndGetAddressOf())));

		pd3dDebug->EnableDebugLayer();
#endif

		// Create the Direct3D device
		HRESULT hr = S_OK;

		hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));

		// Check for maximum feature level support
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_2
		};

		D3D12_FEATURE_DATA_FEATURE_LEVELS	featureLevelInfo = {};
		featureLevelInfo.NumFeatureLevels = _countof(featureLevels);
		featureLevelInfo.pFeatureLevelsRequested = featureLevels;

		DXThrowIfFailed(
			m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

		m_d3dDevice.Reset();

		hr = D3D12CreateDevice(nullptr, featureLevelInfo.MaxSupportedFeatureLevel, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));

		// If device creation is not successful with the given adapter
		if (FAILED(hr))
		{
			ComPtr<IDXGIFactory4>	dxgiFactory{ nullptr };

			DXThrowIfFailed(
				CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())));

			ComPtr<IDXGIAdapter>	warpAdapter{ nullptr };

			DXThrowIfFailed(
				dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.ReleaseAndGetAddressOf())));

			// Fallback to WARP (Windows Advanced Rasterization Platform) Adapter
			hr = D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));
		}
		DXThrowIfFailed(hr);
	}
}
