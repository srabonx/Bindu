#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <wrl/client.h>
#include <d3d12.h>
#include <cstdint>
#include <vector>

#include "CpuDescriptorHeap.h"


namespace BINDU
{
	class D3DRenderDevice;
	class D3DSwapChain;
	class D3DCommandContext;

	using namespace Microsoft::WRL;

	class RenderTexture
	{
		friend class D3DSwapChain;
	public:
		/* rtBuffFormat = Render target buffer format
		 * dsBuffFormat = Depth stencil buffer format
		 */
		RenderTexture(DXGI_FORMAT rtBuffFormat, DXGI_FORMAT dsBuffFormat, std::uint8_t bufferCount, DXGI_SAMPLE_DESC sampleDesc);

		~RenderTexture();

		void						Initialize(std::uint16_t width, std::uint16_t height, const std::shared_ptr<D3DRenderDevice>& pDevice,
												bool isSwapChainBuffer = false);

		void						Begin(const D3DCommandContext& commandContext);

		void						Clear(const D3DCommandContext& commandContext, const float* color, float depth, std::uint8_t stencil) const;
		
		void						End(const D3DCommandContext& commandContext);

		// Reset and resize the buffers to the new size
		void						Resize(std::uint16_t width, std::uint16_t height);

		// Reset current resources
		void						Reset();

		// TODO: Implement this method along with Vec4
		//void						SetOptimizedClearColor(Vec4 color);

		ID3D12Resource*				GetRtBuffer(int index) const;

		ID3D12Resource*				GetCurrentRtBuffer() const;

		ID3D12Resource*				GetDsBuffer() const;

		D3D12_CPU_DESCRIPTOR_HANDLE	GetCurrentRtvCpuHandle() const;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSrvCpuHandle() const;

		D3D12_CPU_DESCRIPTOR_HANDLE	GetRtvCpuHandle(int index) const;

		D3D12_CPU_DESCRIPTOR_HANDLE GetSrvCpuHandle(int index) const;

		D3D12_CPU_DESCRIPTOR_HANDLE	GetDsvCpuHandle() const;

		std::uint8_t				GetBufferCount() const;

		DXGI_FORMAT					GetRenderTargetBufferFormat() const;

		DXGI_FORMAT					GetDepthStencilBufferFormat() const;

		DXGI_SAMPLE_DESC			GetSampleDesc() const;

		std::uint16_t				GetWidth() const;

		std::uint16_t				GetHeight() const;



		void						SetSampleDesc(DXGI_SAMPLE_DESC sampleDesc);

		bool						IsSwapChainBuffer() const;


	private:
		void						TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state);

		void						CreateRTBuffer(std::uint16_t width, std::uint16_t height);

		void						CreateDSBuffer(std::uint16_t width, std::uint16_t height);

	private:

		// Pointer to the render device this texture was initialized with
		std::shared_ptr<D3DRenderDevice>		m_renderDevice{ nullptr };

		// Back buffer count
		std::uint8_t							m_bufferCount{ 2 };

		// Buffer for RenderTarget
		std::vector<ComPtr<ID3D12Resource>>		m_rtBuffers;

		// Buffer for Depth Stencil
		ComPtr<ID3D12Resource>					m_dsBuffer;

		// CpuHeap RTV handle allocation for RenderTarget buffers
		DescriptorHeapAllocation				m_rtvCpuHeapAllocation;

		// CpuHeap SRV handle allocation for RenderTarget buffers
		DescriptorHeapAllocation				m_srvCpuHeapAllocation;

		// CpuHeap DSV handle allocation for RenderTarget buffer
		DescriptorHeapAllocation				m_dsvCpuHeapAllocation;

		// Render target buffer format
		DXGI_FORMAT								m_rtBuffFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Depth stencil buffer format			
		DXGI_FORMAT								m_dsBuffFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Sample description
		DXGI_SAMPLE_DESC						m_sampleDesc;

		// Current index of the RenderTarget buffer
		std::uint8_t							m_currentRtBuffer{ 0 };

		// Current state of the 
		D3D12_RESOURCE_STATES					m_currentState;

		bool									m_isSwapChainBuffer{ false };

		// Width and height of the RenderTexture
		std::uint16_t							m_width{ 0 };

		std::uint16_t							m_height{ 0 };
	};

}

#endif