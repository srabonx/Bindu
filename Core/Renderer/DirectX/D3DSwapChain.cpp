#include "D3DSwapChain.h"

#include "D3DCommandContext.h"
#include "D3DRenderDevice.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "../../../Window/Win32Window.h"
#include "Utility/D3DUtillity.h"
#include "Utility/RenderTexture.h"

namespace BINDU
{
	void D3DSwapChain::Initialize(const std::shared_ptr<D3DCommandContext>& parentContext,
		const std::shared_ptr<RenderTexture>& renderTexture,
		Win32Window* window)
	{
		if (!parentContext || !renderTexture || !window)
			THROW_EXCEPTION(3, "Invalid arguments");

		if (!renderTexture->IsSwapChainBuffer())
			THROW_EXCEPTION(3, "RenderTexture is not initialized as SwapChain buffer");

		CreateSwapChain(parentContext.get(), renderTexture.get(), window);

		CreateRtBuffersFromSwapChainBuffers(renderTexture.get());

		m_parentCommandContext = parentContext;
		m_renderTexture = renderTexture;
		m_window = window;
		m_width = static_cast<std::uint16_t>(window->GetWidth());
		m_height = static_cast<std::uint16_t>(window->GetHeight());
	}

	void D3DSwapChain::PresentRender() const
	{
		DXThrowIfFailed(
			m_dxgiSwapChain->Present(0, 0));
	}

	void D3DSwapChain::Resize()
	{
		auto newWidth = static_cast<std::uint16_t>(m_window->GetWidth());
		auto newHeight = static_cast<std::uint16_t>(m_window->GetHeight());

		if (m_width == newWidth && m_height == newHeight)
			return;

		m_renderTexture->Resize(newWidth, newHeight);

		DXThrowIfFailed(
			m_dxgiSwapChain->ResizeBuffers(m_renderTexture->GetBufferCount(), newWidth, newHeight,
				m_renderTexture->GetRenderTargetBufferFormat(), DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		CreateRtBuffersFromSwapChainBuffers(m_renderTexture.get());

		m_width = newWidth;
		m_height = newHeight;
	}

	D3DCommandContext* D3DSwapChain::GetParentCommandContext() const
	{
		return m_parentCommandContext.get();
	}

	RenderTexture* D3DSwapChain::GetRenderTexture() const
	{
		return m_renderTexture.get();
	}


	void D3DSwapChain::CreateSwapChain(D3DCommandContext* context, RenderTexture* renderTexture, Win32Window* window)
	{
		// Release the previous SwapChain
		m_dxgiSwapChain.Reset();

		auto dxgiFactory = context->GetParentDevice()->GetDXGIFactory();

		m_width = renderTexture->GetWidth();
		m_height = renderTexture->GetHeight();

		DXGI_SWAP_CHAIN_DESC1 scd = {};
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Allow fullscreen
		scd.Format = renderTexture->GetRenderTargetBufferFormat();
		scd.SampleDesc = renderTexture->GetSampleDesc();
		scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		scd.BufferCount = renderTexture->GetBufferCount();
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Width = renderTexture->GetWidth();
		scd.Height = renderTexture->GetHeight();
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
		scfd.Windowed = TRUE;

		// SwapChain uses Command Queue for flush
		DXThrowIfFailed(
			dxgiFactory->CreateSwapChainForHwnd(context->m_commandQueue.Get(), window->GetHandle(), &scd, &scfd,
				nullptr,
				m_dxgiSwapChain.ReleaseAndGetAddressOf()));
	}

	void D3DSwapChain::CreateRtBuffersFromSwapChainBuffers(RenderTexture* renderTexture) const
	{
		for(int i = 0; i< renderTexture->GetBufferCount(); i++)
		{
			auto& buffer = renderTexture->m_rtBuffers[i];

			m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer));
			
		}

		renderTexture->CreateRTBuffer(renderTexture->GetWidth(), renderTexture->GetHeight());
	}
}
