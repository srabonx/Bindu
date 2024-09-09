#ifndef D3DRENDERDEVICE_H
#define D3DRENDERDEVICE_H

#include "IRenderDevice.h"
#include <d3d12.h>
#include <dxgi.h>
#include <wrl/client.h>

#include "Utility/CpuDescriptorHeap.h"
#include "Utility/GpuDescriptorHeap.h"


namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DRenderDevice : public IRenderDevice
	{
	public:
		D3DRenderDevice();

		~D3DRenderDevice();

		void				Initialize() override;

		void				Initialize(IDXGIAdapter* adapter, IDXGIAdapter* warpAdapter);

		void				BeginRender() override;

		void				ClearScreen() override;

		void				EndRender() override;

		ID3D12CommandQueue* GetCommandQueue() const;

		ID3D12Device*		GetD3DDevice() const;

		CpuDescriptorHeap* GetRtvCpuHeap() const;

		CpuDescriptorHeap* GetDsvCpuHeap() const;

		CpuDescriptorHeap* GetCbvSrvUavCpuHeap() const;

		CpuDescriptorHeap* GetSamplerCpuHeap() const;


	private:
		bool				InitD3D(IDXGIAdapter* adapter, IDXGIAdapter* warpAdapter);

	private:

		// Direct3D Device
		ComPtr<ID3D12Device>					m_d3dDevice{ nullptr };

		// DXGI adapter(GPU) this device is created with
		ComPtr<IDXGIAdapter>					m_dxgiAdapter{ nullptr };

		// Direct3D CommandQueue
		ComPtr<ID3D12CommandQueue>				m_commandQueue{ nullptr };

		// Cpu Descriptor Heaps
		std::shared_ptr<CpuDescriptorHeap>		m_rtvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_dsvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_cbvSrvUavCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_samplerCpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_cbvSrvUavGpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_samplerGpuHeap{ nullptr };

	};
}

#endif