#ifndef D3DRENDERDEVICE_H
#define D3DRENDERDEVICE_H

#include "IRenderDevice.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#include "Utility/CpuDescriptorHeap.h"
#include "Utility/FlyFrame.h"
#include "Utility/GpuDescriptorHeap.h"


namespace BINDU
{
	class RenderTexture;
}

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DRenderDevice
	{
	public:
		D3DRenderDevice();

		~D3DRenderDevice();

		void				Initialize();

		void				Close();

		// Check the MSAA quality level for the given buffer format
		std::uint32_t		CheckMSAAQuality(std::uint8_t sampleCount, DXGI_FORMAT bufferFormat) const;


		ID3D12Device*		GetD3DDevice() const;

		IDXGIFactory4*		GetDXGIFactory() const;

		CpuDescriptorHeap*	GetRtvCpuHeap() const;

		CpuDescriptorHeap*	GetDsvCpuHeap() const;

		CpuDescriptorHeap*	GetCbvSrvUavCpuHeap() const;

		CpuDescriptorHeap*	GetSamplerCpuHeap() const;

		GpuDescriptorHeap*	GetCbvSrvUavGpuHeap() const;

		GpuDescriptorHeap* GetSamplerGpuHeap() const;


	private:
		void				InitD3D(IDXGIAdapter* primaryAdapter, IDXGIAdapter* warpAdapter);

		void				InitDxgi();

		void				EnumAdapters(IDXGIFactory* pdxgiFactory);

		void				CheckFeatureLevelSupport(ID3D12Device* pDevice);

	private:

		// Direct3D Device
		ComPtr<ID3D12Device>					m_d3dDevice{ nullptr };

		// DXGI factory
		ComPtr<IDXGIFactory4>					m_dxgiFactory{ nullptr };

		// List of all the available adapters(GPU)
		std::vector<ComPtr<IDXGIAdapter>>		m_adapters;

		// Cpu Descriptor Heaps
		std::shared_ptr<CpuDescriptorHeap>		m_rtvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_dsvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_cbvSrvUavCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_samplerCpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_cbvSrvUavGpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_samplerGpuHeap{ nullptr };

		// Minimum feature level the GPU must support to create this device
		D3D_FEATURE_LEVEL						m_minimumFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		// Maximum feature level the GPU supports
		D3D_FEATURE_LEVEL						m_maximumFeatureLevel;



		// Device states
		bool									m_isPrepared{ false };


	};
}

#endif