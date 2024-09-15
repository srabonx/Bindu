#ifndef D3DCOMMANDCONTEXT_H
#define D3DCOMMANDCONTEXT_H

#include <wrl/client.h>
#include <d3d12.h>
#include <memory>


namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class D3DRenderDevice;
	class D3DFence;
	class D3DSwapChain;

	// Primary object to submit commands to GPU
	class D3DCommandContext
	{
		// This class is friends with D3DSwapChain
		friend class D3DSwapChain;

	public:

		D3DCommandContext();

		~D3DCommandContext();

		void		Initialize(const std::shared_ptr<D3DRenderDevice>& parentDevice);

		void		PrepareForCommands(ID3D12CommandAllocator* commandAllocator) const;

		void		SetViewport(std::uint8_t numOfViewport, const D3D12_VIEWPORT* viewports) const;

		void		SetScissorRect(std::uint8_t numOfRect, const D3D12_RECT* rects) const;

		void		ExecuteCommands() const;

		// Mark commands up to this fence point
		void		Signal(const D3DFence* fence) const;

		// Wait for GPU to complete commands up to this fence point
		void		WaitForGpu(const D3DFence* fence) const;


		D3DRenderDevice* GetParentDevice() const;

		ComPtr<ID3D12GraphicsCommandList>	GetCommandList() const;

		ComPtr<ID3D12CommandAllocator>		GetCommandAllocator() const;

		ComPtr<ID3D12CommandQueue>			GetCommandQueue() const;

	private:
		void		CreateCommandObjects(const D3DRenderDevice* renderDevice);

	private:
		
		// Shared pointer to the RenderDevice that created this CommandContext.
		std::shared_ptr<D3DRenderDevice>	m_parentDevice{ nullptr };
		
		// The CommandQueue interface. The GPU executes commands from this Queue.
		ComPtr<ID3D12CommandQueue>			m_commandQueue{ nullptr };

		// The CommandList interface. It allocates commands to the CommandAllocator 
		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		// The CommandAllocator interface. It allocates commands from CommandList
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator{ nullptr };
	};
}

#endif