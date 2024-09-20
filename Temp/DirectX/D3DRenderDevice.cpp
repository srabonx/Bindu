#include "D3DRenderDevice.h"

#include <dxgi1_3.h>

#include "Utility/D3DUtillity.h"
#include "Utility/RenderTexture.h"
#include <dxgidebug.h>

namespace BINDU
{
	D3DRenderDevice::D3DRenderDevice()
	{
	}


	D3DRenderDevice::~D3DRenderDevice()
	{

	}

	void D3DRenderDevice::Initialize()
	{
		InitDxgi();

		EnumAdapters(m_dxgiFactory.Get());

		ComPtr<IDXGIAdapter>	warpAdapter{ nullptr };

		m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.ReleaseAndGetAddressOf()));

		InitD3D(m_adapters[1].Get(), warpAdapter.Get());

		CheckFeatureLevelSupport(m_d3dDevice.Get());

		m_rtvCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_dsvCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_cbvSrvUavCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_samplerCpuHeap = std::make_shared<CpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

		m_cbvSrvUavGpuHeap = std::make_shared<GpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_samplerGpuHeap = std::make_shared<GpuDescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}

	void D3DRenderDevice::Close()
	{
#if defined(DEBUG) || defined(_DEBUG)
		ComPtr<IDXGIDebug1> pdxgiDebug{ nullptr };
		DXThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(pdxgiDebug.ReleaseAndGetAddressOf())));
		DXThrowIfFailed(pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL));
#endif
	}

	std::uint32_t D3DRenderDevice::CheckMSAAQuality(std::uint8_t sampleCount, DXGI_FORMAT bufferFormat) const
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaSupport = {};
		msaaSupport.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;

		msaaSupport.Format = bufferFormat;

		msaaSupport.SampleCount = sampleCount;

		DXThrowIfFailed(
		m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaSupport, sizeof(msaaSupport)));

		return msaaSupport.NumQualityLevels;
	}


	ID3D12Device* D3DRenderDevice::GetD3DDevice() const
	{
		return m_d3dDevice.Get();
	}

	IDXGIFactory4* D3DRenderDevice::GetDXGIFactory() const
	{
		return m_dxgiFactory.Get();
	}

	CpuDescriptorHeap* D3DRenderDevice::GetRtvCpuHeap() const
	{
		return m_rtvCpuHeap.get();
	}

	CpuDescriptorHeap* D3DRenderDevice::GetDsvCpuHeap() const
	{
		return m_dsvCpuHeap.get();
	}

	CpuDescriptorHeap* D3DRenderDevice::GetCbvSrvUavCpuHeap() const
	{
		return m_cbvSrvUavCpuHeap.get();
	}

	CpuDescriptorHeap* D3DRenderDevice::GetSamplerCpuHeap() const
	{
		return m_samplerCpuHeap.get();
	}

	GpuDescriptorHeap* D3DRenderDevice::GetCbvSrvUavGpuHeap() const
	{
		return m_cbvSrvUavGpuHeap.get();
	}

	GpuDescriptorHeap* D3DRenderDevice::GetSamplerGpuHeap() const
	{
		return m_samplerGpuHeap.get();
	}

	void D3DRenderDevice::InitD3D(IDXGIAdapter* primaryAdapter, IDXGIAdapter* warpAdapter)
	{
#if defined(DEBUG) || defined(_DEBUG)
		// Enabling the debug layer
		ComPtr<ID3D12Debug6>	pd3dDebug{ nullptr };

		DXThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(pd3dDebug.ReleaseAndGetAddressOf())));

		pd3dDebug->EnableDebugLayer();
#endif

		// Create the Direct3D device
		HRESULT hr = S_OK;

		hr = D3D12CreateDevice(primaryAdapter, m_minimumFeatureLevel, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));

		// If device creation is not successful with the given adapter
		if(FAILED(hr))
		{
			// Fallback to WARP (Windows Advanced Rasterization Platform) Adapter
			hr = D3D12CreateDevice(warpAdapter, m_minimumFeatureLevel, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));
		}

		DXThrowIfFailed(hr);
	}

	void D3DRenderDevice::InitDxgi()
	{
		UINT factoryFlag = 0;

		// Enable Debug layer if in DEBUG mode
#if defined (DEBUG) || defined (_DEBUG)
		factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif

		// Create the DXGI Factory
		DXThrowIfFailed(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

	}

	void D3DRenderDevice::EnumAdapters(IDXGIFactory* pdxgiFactory)
	{
		if (!pdxgiFactory)
			return;

		ComPtr<IDXGIAdapter> padapter{ nullptr };

		UINT index{ 0 };

		// Add all the available adapters
		while (pdxgiFactory->EnumAdapters(index, padapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			m_adapters.push_back(padapter);

			++index;
		}
	}

	void D3DRenderDevice::CheckFeatureLevelSupport(ID3D12Device* pDevice)
	{
		if (!pDevice)
			return;

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
			pDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

		// Maximum feature level support
		m_maximumFeatureLevel = featureLevelInfo.MaxSupportedFeatureLevel;
	}

}
