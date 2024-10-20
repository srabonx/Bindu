#ifndef BINDU_DIRECTX_GRAPHICS_CONTEXT_H
#define BINDU_DIRECTX_GRAPHICS_CONTEXT_H

#include <dxgi1_4.h>
#include <wrl/client.h>


#include "D3DFence.h"
#include "DirectXRenderTexture.h"
#include "FlyFrame.h"
#include "../GraphicsContext.h"
#include "../../../Window/IWindow.h"
#include "../../../Utility/Common/CommonUtility.h"


namespace BINDU
{
	class DirectXRenderer;
}

namespace BINDU
{
	using namespace Microsoft::WRL;

	class DirectXGraphicsContext : public GraphicsContext
	{
		friend class DirectXCommandList;
	public:
		DirectXGraphicsContext(IWindow* window, const Ref<DirectXRenderer>& directXRenderer);

		~DirectXGraphicsContext() override;

		void Initialize(const GraphicsContextProperties& properties) override;

		void PrepareForCommands() override;

		Ref<CommandList> GetCommandList() override;

		void ExecuteCommands() override;

		void Present() override;

		void Resize(std::uint16_t newWidth, std::uint16_t newHeight) override;

		DirectXRenderTexture* GetCurrentSwapChainRT() const;
	private:

		void InitDXGI();

		void CreateSwapChain();

		void GetSwapChainBuffers();

	private:

		Ref<DirectXRenderer>		m_directXRenderer;

		Ref<DirectXCommandList>		m_commandList;

		Ref<D3DCommandContext>		m_commandContext{ nullptr };

		GraphicsContextProperties	m_properties;

		IWindow*		m_window{ nullptr };

		std::uint16_t	m_width{ 0 };

		std::uint16_t	m_height{ 0 };

		ComPtr<IDXGIFactory4>	m_dxgiFactory{ nullptr };

		ComPtr<IDXGISwapChain3>	m_dxgiSwapChain{ nullptr };

		std::vector<ComPtr<ID3D12Resource>>	m_swapChainBuffers;

		std::vector<Ref<DirectXRenderTexture>> m_renderTextures;

		Ref<Texture>			m_depthStencilBuffer{nullptr};

		Scoped<D3DFence> m_fence{ nullptr };


		DXGI_FORMAT		m_backBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	};
}

#endif