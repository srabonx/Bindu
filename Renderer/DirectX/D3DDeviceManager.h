#ifndef D3DDEVICEMANAGER_H
#define D3DDEVICEMANAGER_H

#include <wrl/client.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>
#include <string>

#include "DescriptorHeapManager.h"
#include "DirectXConstantBuffer.h"
#include "../../Utility/Common/CommonUtility.h"


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

		ComPtr<ID3D12Resource>	CreateCommitedTex2D(std::uint64_t width, std::uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags, D3D12_HEAP_FLAGS heapFlag = D3D12_HEAP_FLAG_NONE, const D3D12_CLEAR_VALUE* optimizedClearVal = nullptr) const;

		ComPtr<ID3D12Resource>	CreateCommitedBuffer(std::uint64_t width, D3D12_HEAP_TYPE heapType, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_STATES initialState) const;

		ComPtr<ID3D12Resource>	CreateDefaultBuffer(const void* initData, UINT64 byteSize, ID3D12GraphicsCommandList* commandList, ComPtr<ID3D12Resource>& uploadBuffer) const;

		ComPtr<ID3D12PipelineState>	CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& psd) const;

		ComPtr<ID3D12PipelineState> CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC& psd) const;

		ComPtr<ID3D12RootSignature>	CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc) const;

		[[nodiscard]] DescriptorHeapAllocation	CreateDepthStencilView(ID3D12Resource* depthStencilResource, const D3D12_DEPTH_STENCIL_VIEW_DESC& depthStencilDesc) const;

		[[nodiscard]] DescriptorHeapAllocation	CreateRenderTargetView(ID3D12Resource* renderTargetResource, const D3D12_RENDER_TARGET_VIEW_DESC& rtvDesc) const;

		[[nodiscard]] DescriptorHeapAllocation	CreateShaderResourceView(ID3D12Resource* srvResource, const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc) const;

		Ref<DirectXConstantBuffer>	CreateConstantBuffer(std::uint32_t elementCount, std::uint32_t elementByteSize) const;

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