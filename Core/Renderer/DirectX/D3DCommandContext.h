#ifndef D3DCOMMANDCONTEXT_H
#define D3DCOMMANDCONTEXT_H

#include <wrl/client.h>
#include <d3d12.h>
#include <memory>


namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class D3DDeviceManager;
	class D3DFence;

	// Primary object to submit commands to GPU
	class D3DCommandContext
	{
	public:

		D3DCommandContext();

		~D3DCommandContext();

		void		Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

		void		PrepareForCommands(ID3D12CommandAllocator* commandAllocator) const;

		void		ExecuteCommands() const;

		// Mark commands up to this fence point
		void		Signal(const D3DFence* fence) const;

		// Wait for GPU to complete commands up to this fence point
		void		WaitForGpu(const D3DFence* fence) const;

		D3DDeviceManager*					GetDeviceManager() const;

		ComPtr<ID3D12GraphicsCommandList>	GetCommandList() const;

		ComPtr<ID3D12CommandAllocator>		GetCommandAllocator() const;

		ComPtr<ID3D12CommandQueue>			GetCommandQueue() const;

	private:
		void		CreateCommandObjects(const D3DDeviceManager* deviceManager);

	private:
		
		// Shared pointer to the RenderDevice that created this CommandContext.
		std::shared_ptr<D3DDeviceManager>	m_deviceManager{ nullptr };
		
		// The CommandQueue interface. The GPU executes commands from this Queue.
		ComPtr<ID3D12CommandQueue>			m_commandQueue{ nullptr };

		// The CommandList interface. It allocates commands to the CommandAllocator 
		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		// The CommandAllocator interface. It allocates commands from CommandList
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator{ nullptr };
	};
}

#endif