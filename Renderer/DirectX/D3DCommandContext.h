#ifndef D3DCOMMANDCONTEXT_H
#define D3DCOMMANDCONTEXT_H

#include <wrl/client.h>
#include <d3d12.h>
#include <memory>
#include <string>


namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class D3DDeviceManager;
	class D3DFence;
	class RenderTexture;
	class GameObject;
	class D3DPipelineStateManager;
	class DepthStencilBuffer;


	// Primary object to submit commands to GPU
	class D3DCommandContext
	{
	public:

		D3DCommandContext();

		~D3DCommandContext();

		void		Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

		void		PrepareForCommands(ID3D12CommandAllocator* commandAllocator) const;

		void		ExecuteCommands() const;

		// Begin rendering on the given render texture
		void		Begin(RenderTexture* renderTexture, DepthStencilBuffer* depthStencilBuffer);

		// Bind Pipeline State
		void		BindPipeline(const D3DPipelineStateManager& pipelineManager, const std::string& pipelineName, const std::string& rootSigName) const;

		void		BindPipeline(ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature) const;

		void		End();

		// Mark commands up to this fence point
		void		Signal(const D3DFence* fence) const;

		// Wait for GPU to complete commands up to this fence point
		void		WaitForGpu(const D3DFence* fence) const;

		// Wait for GPU to complete commands up to this fence value point
		void		WaitForGpu(const D3DFence* fence, std::uint64_t fenceValue) const;

		RenderTexture*						GetCurrentlyBoundRenderTexture() const;

		D3DDeviceManager*					GetDeviceManager() const;

		ComPtr<ID3D12GraphicsCommandList>	GetCommandList() const;

		ComPtr<ID3D12CommandAllocator>		GetCommandAllocator() const;

		ComPtr<ID3D12CommandQueue>			GetCommandQueue() const;

	private:
		void		CreateCommandObjects(const D3DDeviceManager* deviceManager);

	private:
		
		// Shared pointer to the DeviceManager that created this CommandContext.
		std::shared_ptr<D3DDeviceManager>	m_deviceManager{ nullptr };
		
		// The CommandQueue interface. The GPU executes commands from this Queue.
		ComPtr<ID3D12CommandQueue>			m_commandQueue{ nullptr };

		// The CommandList interface. It allocates commands to the CommandAllocator 
		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		// The CommandAllocator interface. It allocates commands from CommandList
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator{ nullptr };

		// Currently bound render texture for rendering
		RenderTexture*						m_tempRenderTexture{ nullptr };

		bool								m_beginEndPair{ false };
	};
}

#endif