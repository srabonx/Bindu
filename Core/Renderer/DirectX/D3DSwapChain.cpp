#include "D3DSwapChain.h"

#include "D3DDeviceManager.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "D3DUtillity.h"
#include "../../Resources/RenderTexture.h"

namespace BINDU
{
	void D3DSwapChain::Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager,
		HWND hwnd, std::uint16_t width, std::uint16_t height, std::uint8_t bufferCount)
	{
		if (!parentDeviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		m_width = width;
		m_height = height;
		m_bufferCount = bufferCount;
		m_parentDeviceManager = parentDeviceManager;
		m_sampleDesc = { 1,0 };

		m_renderTextures.resize(bufferCount);

		InitDXGI();

		CreateSwapChain(hwnd, width, height, bufferCount);

		CreateBackBuffers();

	}

	void D3DSwapChain::PresentRender(std::uint8_t syncInterval, std::uint8_t flags) const
	{
		DXThrowIfFailed(
			m_dxgiSwapChain->Present(syncInterval, flags));
	}

	void D3DSwapChain::Resize(std::uint16_t width, std::uint16_t height)
	{

		if (m_width == width && m_height == height)
			return;

		for (auto& rt : m_renderTextures)
			rt.reset();

		DXThrowIfFailed(
			m_dxgiSwapChain->ResizeBuffers(m_bufferCount, width, height,
				m_backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));


		CreateBackBuffers();

		m_width = width;
		m_height = height;
	}

	RenderTexture* D3DSwapChain::GetRenderTarget() const
	{
		return m_renderTextures[m_dxgiSwapChain->GetCurrentBackBufferIndex()].get();
	}

	void D3DSwapChain::InitDXGI()
	{
		UINT factoryFlag = 0;

		// Enable Debug layer if in DEBUG mode
#if defined (DEBUG) || defined (_DEBUG)
		factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif

		// Create the DXGI Factory
		DXThrowIfFailed(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
	}


	void D3DSwapChain::CreateSwapChain(HWND hwnd, std::uint16_t width, std::uint16_t height, std::uint8_t bufferCount)
	{
		// Release the previous SwapChain
		m_dxgiSwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC1 scd = {};
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Allow fullscreen
		scd.Format = m_backBufferFormat;
		scd.SampleDesc = m_sampleDesc;
		scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		scd.BufferCount = bufferCount;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Width = width;
		scd.Height = height;
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
		scfd.Windowed = TRUE;

		auto commandQueue = m_parentDeviceManager->GetCommandQueue();

		// Temporary SwapChain
		ComPtr<IDXGISwapChain1> swapChain{ nullptr };

		// SwapChain uses Command Queue for flush
		DXThrowIfFailed(
			m_dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &scd, &scfd,
				nullptr,
				swapChain.ReleaseAndGetAddressOf()));

		swapChain.As(&m_dxgiSwapChain);
	}

	void D3DSwapChain::CreateBackBuffers()
	{
		std::vector<ComPtr<ID3D12Resource>> renderTargets(m_bufferCount);

		for (int i = 0; i < m_bufferCount; i++)
		{
			DXThrowIfFailed(
				m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].ReleaseAndGetAddressOf())));
		}

		for (int i = 0; i < m_bufferCount; i++)
		{
			m_renderTextures[i] = std::make_shared<RenderTexture>(renderTargets[i]);

			ResourceProperties rcProp = {};

			m_renderTextures[i]->Initialize(rcProp, m_parentDeviceManager);

			m_renderTextures[i]->CreateRenderTargetView(m_backBufferFormat);
		}
	}
}
