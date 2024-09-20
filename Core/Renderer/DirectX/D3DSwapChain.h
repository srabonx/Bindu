#ifndef D3DSWAPCHAIN_H
#define D3DSWAPCHAIN_H

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <memory>

namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class RenderTexture;

	class D3DDeviceManager;

	class D3DSwapChain
	{
	public:
		D3DSwapChain() = default;

		~D3DSwapChain() = default;

		void				Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager,
							HWND hwnd, std::uint16_t width, std::uint16_t height, std::uint8_t bufferCount = 2);

		void				PresentRender(std::uint8_t syncInterval = 1, std::uint8_t flags = 0) const;

		// Resize buffers to the new size
		void				Resize(std::uint16_t width, std::uint16_t height);

		RenderTexture*		GetRenderTarget() const;

	private:

		void				InitDXGI();

		void				CreateSwapChain(HWND hwnd, std::uint16_t width, std::uint16_t height, std::uint8_t bufferCount);

		void				CreateBackBuffers();

	private:

		// Parent Device Manager for this SwapChain
		std::shared_ptr<D3DDeviceManager>	m_parentDeviceManager{ nullptr };

		std::shared_ptr<RenderTexture>		m_renderTarget{ nullptr };

		// Underlying DXGI Factory
		ComPtr<IDXGIFactory4>				m_dxgiFactory{ nullptr };

		// Underlying SwapChain interface
		ComPtr<IDXGISwapChain3>				m_dxgiSwapChain{ nullptr };

		std::uint16_t						m_width{ 0 };

		std::uint16_t						m_height{ 0 };

		std::uint8_t						m_bufferCount{ 0 };

		DXGI_FORMAT							m_backBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };

		DXGI_SAMPLE_DESC					m_sampleDesc;

	};
}

#endif