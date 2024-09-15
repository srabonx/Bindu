#ifndef D3DSWAPCHAIN_H
#define D3DSWAPCHAIN_H

#include <dxgi1_4.h>
#include <wrl/client.h>
#include <memory>

namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class D3DCommandContext;
	class RenderTexture;
	class Win32Window;

	class D3DSwapChain
	{
	public:
		D3DSwapChain() = default;

		~D3DSwapChain() = default;

		void				Initialize(const std::shared_ptr<D3DCommandContext>& parentContext,
										const std::shared_ptr<RenderTexture>& renderTexture,
										 Win32Window* window);

		void				PresentRender() const;

		// Resize buffers to the new size
		void				Resize();

		D3DCommandContext* GetParentCommandContext() const;

		RenderTexture*		GetRenderTexture() const;

	private:

		void				CreateSwapChain(D3DCommandContext* context, RenderTexture* renderTexture, Win32Window* window);

		void				CreateRtBuffersFromSwapChainBuffers(RenderTexture* renderTexture) const;

	private:

		// Pointer to the CommandContext that Initialized this SwapChain
		std::shared_ptr<D3DCommandContext>	m_parentCommandContext{ nullptr };

		// Pointer to the RenderTexture this SwapChain uses as its buffer.
		std::shared_ptr<RenderTexture>		m_renderTexture{ nullptr };

		// Pointer to the Window this SwapChain got created for
		Win32Window*						m_window{ nullptr };

		// Underlying SwapChain interface
		ComPtr<IDXGISwapChain1>				m_dxgiSwapChain{ nullptr };

		std::uint16_t						m_width{ 0 };

		std::uint16_t						m_height{ 0 };

	};
}

#endif