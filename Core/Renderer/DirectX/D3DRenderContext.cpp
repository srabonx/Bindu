#include "D3DRenderContext.h"

#include "D3DRenderDevice.h"
#include "Utility/D3DUtillity.h"

namespace BINDU
{
	D3DRenderContext::D3DRenderContext(Win32Window* window) : m_window(window)
	{
	}

	D3DRenderContext::~D3DRenderContext()
	{
	}

	void D3DRenderContext::Initialize()
	{

		// Initialize DXGI
		InitDXGI();

		// Enumerate the adapters
		EnumAdapters(m_dxgiFactory.Get());

	}

	void D3DRenderContext::CreateSwapChain(D3DRenderDevice* renderDevice, RenderTexture* renderTexture)
	{
		if (!renderDevice)
			THROW_EXCEPTION(3, "Invalid RenderDevice");

		if (!renderTexture)
			THROW_EXCEPTION(3, "Invalid RenderTexture");

		CreateSwapChain(m_dxgiFactory.Get(), renderDevice, renderTexture);


		m_renderDevice.reset(renderDevice);

		m_renderTexture.reset(renderTexture);

	}

	void D3DRenderContext::Resize(std::uint16_t width, std::uint16_t height)
	{
		CreateSwapChain(m_dxgiFactory.Get(), m_renderDevice.get(), m_renderTexture.get());

		m_renderTexture->Resize(width, height);
	}

	IDXGIAdapter* D3DRenderContext::GetAdapter(int index) const
	{
		if (index < 0 || index >= m_adapters.size())
			return nullptr;

		return m_adapters[index].Get();
	}

	IDXGIAdapter* D3DRenderContext::GetWarpAdapter()
	{
		if (m_dxgiFactory)
		{
			m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(m_warpAdapter.ReleaseAndGetAddressOf()));
			return m_warpAdapter.Get();
		}

		return nullptr;
	}

	void D3DRenderContext::SetMSAADesc(DXGI_SAMPLE_DESC sampleDesc)
	{
		m_renderTexture->SetSampleDesc(sampleDesc);
		CreateSwapChain(m_dxgiFactory.Get(), m_renderDevice.get(), m_renderTexture.get());
	}



	// Private methods

	void D3DRenderContext::InitDXGI()
	{
		UINT factoryFlag = 0;

		// Enable Debug layer if in DEBUG mode
#if defined (DEBUG) || defined (_DEBUG)
		factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif

		// Create the DXGI Factory
		DXThrowIfFailed(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
	}

	void D3DRenderContext::EnumAdapters(IDXGIFactory* pdxgiFactory)
	{
		if (!pdxgiFactory)
			return;

		ComPtr<IDXGIAdapter> padapter{ nullptr };

		UINT index{ 0 };

		// Add all the available adapters
		while(pdxgiFactory->EnumAdapters(index, padapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			m_adapters.push_back(padapter);

			++index;
		}
	}

	void D3DRenderContext::CreateSwapChain(IDXGIFactory* pdxgiFactory, D3DRenderDevice* renderDevice, RenderTexture* renderTexture)
	{
		// Release the previous SwapChain
		m_dxgiSwapChain.Reset();

		if (!pdxgiFactory || !renderDevice || !renderTexture)
			THROW_EXCEPTION(3, "Invalid argument");

		ComPtr<IDXGIFactory4> pfactory{ nullptr };

		pdxgiFactory->QueryInterface(IID_PPV_ARGS(pfactory.ReleaseAndGetAddressOf()));

		DXGI_SWAP_CHAIN_DESC1 scd = {};
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Allow fullscreen
		scd.Format = renderTexture->GetRenderTargetBufferFormat();
		scd.SampleDesc = renderTexture->GetSampleDesc();
		scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		scd.BufferCount = renderTexture->GetBufferCount();
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Width = m_window->GetWidth();
		scd.Height = m_window->GetHeight();
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
		scfd.Windowed = TRUE;

		// SwapChain uses Command Queue for flush
		DXThrowIfFailed(
			pfactory->CreateSwapChainForHwnd(renderDevice->GetCommandQueue(), m_window->GetHandle(), &scd, &scfd, nullptr,
				m_dxgiSwapChain.ReleaseAndGetAddressOf()));
	}
}
