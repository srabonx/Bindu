#include "D3DDeviceManager.h"

#include "CpuDescriptorHeap.h"
#include "D3DUtillity.h"
#include "GpuDescriptorHeap.h"
#include <dxgidebug.h>

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

	void D3DDeviceManager::CreateTex2DResource(std::uint64_t width, std::uint64_t height, DXGI_SAMPLE_DESC sampleDesc, 
		const Resource* resource, D3D12_HEAP_FLAGS heapFlags, D3D12_HEAP_TYPE heapType) const
	{
		auto d3dResource = resource->GetResource();

		CD3DX12_HEAP_PROPERTIES heapProp(heapType);

		D3D12_HEAP_FLAGS l_heapFlags = heapFlags;

		D3D12_RESOURCE_DESC	rsd = CD3DX12_RESOURCE_DESC::Tex2D(resource->GetFormat(), width, height, 1, 1, sampleDesc.Count,
			sampleDesc.Quality, resource->GetFlags());

		DXThrowIfFailed(
			m_d3dDevice->CreateCommittedResource(&heapProp, l_heapFlags, &rsd, resource->GetState(),
				nullptr, IID_PPV_ARGS(d3dResource.GetAddressOf())));
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
