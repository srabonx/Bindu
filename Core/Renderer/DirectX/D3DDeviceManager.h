#ifndef D3DDEVICEMANAGER_H
#define D3DDEVICEMANAGER_H

#include <wrl/client.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>


namespace BINDU
{
	class CpuDescriptorHeap;
	class GpuDescriptorHeap;
	class Resource;
	
	using namespace Microsoft::WRL;

	// Manages the creation of different objects related to rendering
	class D3DDeviceManager
	{
	public:

		D3DDeviceManager() = default;
		~D3DDeviceManager() = default;

		void					Initialize();

		void					Close();

		void					CreateTex2DResource(std::uint64_t width, std::uint64_t height, DXGI_SAMPLE_DESC sampleDesc, const Resource* resource,
													D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE,
													D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT) const;

		ID3D12Device*			GetD3DDevice() const;

		ID3D12CommandQueue*		GetCommandQueue() const;

		CpuDescriptorHeap*		GetRtvCpuHeap() const;

		CpuDescriptorHeap*		GetDsvCpuHeap() const;

		CpuDescriptorHeap*		GetCbvSrvUavCpuHeap() const;

		CpuDescriptorHeap*		GetSamplerCpuHeap() const;

		GpuDescriptorHeap*		GetCbvSrvUavGpuHeap() const;

		GpuDescriptorHeap*		GetSamplerGpuHeap() const;


	private:
		void					InitD3D();


	private:

		// Underlying Direct3D device
		ComPtr<ID3D12Device>					m_d3dDevice{ nullptr };

		// Command Queue
		ComPtr<ID3D12CommandQueue>				m_commandQueue{ nullptr };


		std::shared_ptr<CpuDescriptorHeap>		m_rtvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_dsvCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_cbvSrvUavCpuHeap{ nullptr };

		std::shared_ptr<CpuDescriptorHeap>		m_samplerCpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_cbvSrvUavGpuHeap{ nullptr };

		std::shared_ptr<GpuDescriptorHeap>		m_samplerGpuHeap{ nullptr };



	};
}

#endif