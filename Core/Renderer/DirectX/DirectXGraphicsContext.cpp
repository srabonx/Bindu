#include "DirectXGraphicsContext.h"

#include "D3DCommandContext.h"
#include "D3DUtillity.h"
#include "DirectXRenderer.h"
#include "GpuDescriptorHeap.h"
#include "../../../Debug/Profiler.h"
#include "DirectXCommandList.h"

namespace BINDU
{

	DirectXGraphicsContext::DirectXGraphicsContext(IWindow* window, const Ref<DirectXRenderer>& directXRenderer) : m_directXRenderer(directXRenderer) , m_window(window)
	{
		m_commandContext = directXRenderer->GetCommandContext();

		m_commandList = CreateRef<DirectXCommandList>(m_commandContext->GetCommandList());
	}

	DirectXGraphicsContext::~DirectXGraphicsContext()
	{
	}

	void DirectXGraphicsContext::InitDXGI()
	{
		UINT factoryFlag = 0;

		// Enable Debug layer if in DEBUG mode
#if defined (DEBUG) || defined (_DEBUG)
		factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif

		// Create the DXGI Factory
		DXThrowIfFailed(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
	}

	void DirectXGraphicsContext::CreateSwapChain()
	{
		// Release the previous SwapChain
		m_dxgiSwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC1 scd = {};
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// Allow fullscreen
		scd.Format = m_backBufferFormat;
		scd.SampleDesc.Count = m_properties.MSAASampleCount;
		scd.SampleDesc.Quality = m_properties.MSAASampleQuality;
		scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		scd.BufferCount = m_properties.BackBufferCount;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Width = m_window->GetWidth();
		scd.Height = m_window->GetHeight();
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
		scfd.Windowed = TRUE;

		auto commandQueue = m_directXRenderer->m_deviceManager->GetCommandQueue();

		// Temporary SwapChain
		ComPtr<IDXGISwapChain1> swapChain{ nullptr };

		// SwapChain uses Command Queue for flush
		DXThrowIfFailed(
			m_dxgiFactory->CreateSwapChainForHwnd(commandQueue, m_window->GetHandle(), &scd, &scfd,
				nullptr,
				swapChain.ReleaseAndGetAddressOf()));

		swapChain.As(&m_dxgiSwapChain);


		GetSwapChainBuffers();
	}

	void DirectXGraphicsContext::GetSwapChainBuffers()
	{
		TextureSpecification specification;
		specification.Format = TextureFormat::D24_UNORM_S8_UINT;
		specification.Width = m_width;
		specification.Height = m_height;

		m_depthStencilBuffer = m_directXRenderer->CreateDepthStencilTexture(specification);

		specification.Format = TextureFormat::RGBA8_UNORM;

		ComPtr<ID3D12Resource> swapChainBuffer{ nullptr };
		for (int i = 0; i < m_properties.BackBufferCount; i++)
		{
			m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(swapChainBuffer.ReleaseAndGetAddressOf()));

			auto rt = m_directXRenderer->CreateRenderTexture(swapChainBuffer, m_depthStencilBuffer.get(), specification);

			m_renderTextures.emplace_back(std::static_pointer_cast<DirectXRenderTexture>(rt));

			m_swapChainBuffers.emplace_back(swapChainBuffer);
		}

	}

	void DirectXGraphicsContext::Initialize(const GraphicsContextProperties& properties)
	{
		BINDU_PROFILE_FUNCTION();

		m_properties = properties;

		m_backBufferFormat = D3DUtility::GetDXGIFormatFromTextureFormat(properties.BackBufferFormat);

		InitDXGI();

		CreateSwapChain();

		m_fence = CreateScoped<D3DFence>();

		m_fence->Initialize(m_directXRenderer->m_deviceManager);

	}

	void DirectXGraphicsContext::PrepareForCommands()
	{
		m_commandContext->PrepareForCommands(nullptr);
	}

	Ref<CommandList> DirectXGraphicsContext::GetCommandList()
	{
		return std::static_pointer_cast<CommandList>(m_commandList);
	}

	void DirectXGraphicsContext::ExecuteCommands()
	{
		m_commandContext->ExecuteCommands();
	}

	void DirectXGraphicsContext::Present()
	{
		DXThrowIfFailed(
			m_dxgiSwapChain->Present(0, 0));


		m_fence->Advance();

		m_commandContext->WaitForGpu(m_fence.get());

		m_directXRenderer->m_deviceManager->GetCbvSrvUavGpuHeap()->ReleaseStaleAllocations(m_fence->GetCurrentValue());
	}

	void DirectXGraphicsContext::Resize(std::uint16_t newWidth, std::uint16_t newHeight)
	{
		if (m_width == newWidth && m_height == newHeight)
			return;

		m_depthStencilBuffer.reset();
		m_renderTextures.clear();
		m_swapChainBuffers.clear();

		DXThrowIfFailed(
			m_dxgiSwapChain->ResizeBuffers(m_properties.BackBufferCount, newWidth, newHeight, m_backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		GetSwapChainBuffers();

		m_width = newWidth;
		m_height = newHeight;
	}

	DirectXRenderTexture* DirectXGraphicsContext::GetCurrentSwapChainRT() const
	{
		return m_renderTextures[m_dxgiSwapChain->GetCurrentBackBufferIndex()].get();
	}
}
