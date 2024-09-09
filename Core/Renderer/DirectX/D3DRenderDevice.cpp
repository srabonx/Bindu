#include "D3DRenderDevice.h"

#include "Utility/D3DUtillity.h"

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
	}

	void D3DRenderDevice::Initialize(IDXGIAdapter* adapter, IDXGIAdapter* warpAdapter)
	{
		m_dxgiAdapter = adapter;

		InitD3D(adapter, warpAdapter);
	}

	void D3DRenderDevice::BeginRender()
	{
	}

	void D3DRenderDevice::ClearScreen()
	{
	}

	void D3DRenderDevice::EndRender()
	{
	}

	ID3D12CommandQueue* D3DRenderDevice::GetCommandQueue() const
	{
		return m_commandQueue.Get();
	}

	ID3D12Device* D3DRenderDevice::GetD3DDevice() const
	{
		return m_d3dDevice.Get();
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

	bool D3DRenderDevice::InitD3D(IDXGIAdapter* adapter, IDXGIAdapter* warpAdapter)
	{
#if defined(DEBUG) || defined(_DEBUG)
		// Enabling the debug layer
		ComPtr<ID3D12Debug>	pd3dDebug{ nullptr };

		DXThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(pd3dDebug.ReleaseAndGetAddressOf())));

		pd3dDebug->EnableDebugLayer();
#endif

		// Create the Direct3D device
		HRESULT hr = S_OK;

		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));

		// If device creation is not successful with the given adapter
		if(FAILED(hr))
		{
			// Fallback to WARP (Windows Advanced Rasterization Platform) Adapter
			hr = D3D12CreateDevice(warpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));
		}

		DXThrowIfFailed(hr);

		// Check for maximum feature level support
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_2
		};

		D3D12_FEATURE_DATA_FEATURE_LEVELS	featureLevelInfo = { 0 };
		featureLevelInfo.NumFeatureLevels = _countof(featureLevels);
		featureLevelInfo.pFeatureLevelsRequested = featureLevels;

		DXThrowIfFailed(
			m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));



	}
}
