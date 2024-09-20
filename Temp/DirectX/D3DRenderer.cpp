#include "D3DRenderer.h"

#include <dxgidebug.h>

#include "Utility/D3DUtillity.h"

namespace BINDU
{
	D3DRenderer::D3DRenderer(Win32Window* window) : m_window(window)
	{
		m_renderTexture = std::make_shared<RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT,
			2, DXGI_SAMPLE_DESC{ 1,0 });

		m_renderDevice = std::make_shared<D3DRenderDevice>();

		m_viewport = { 0,0, static_cast<FLOAT>(window->GetWidth()), static_cast<FLOAT>(window->GetHeight())};
		m_rect = { 0,0, static_cast<LONG>(window->GetWidth()),static_cast<LONG>(window->GetHeight()) };
	}

	D3DRenderer::~D3DRenderer()
	{

	}

	void D3DRenderer::Initialize()
	{
		m_renderDevice->Initialize();

		m_renderDevice->CreateSwapChainForHwnd(m_window->GetHandle(), m_window->GetWidth(), m_window->GetHeight(), m_renderTexture->GetRenderTargetBufferFormat(),
			m_renderTexture->GetSampleDesc(), m_renderTexture->GetBufferCount());

		m_renderTexture->Initialize(m_window->GetWidth(), m_window->GetHeight(), m_renderDevice.get(), true);


		// Fly Frame
		m_flyFrame = std::make_unique<FlyFrame>(3);

		m_flyFrame->Initialize(m_renderDevice.get());

		m_renderDevice->ExecuteCommands();

		m_renderDevice->WaitForGPU();

	}

	void D3DRenderer::BeginRender()
	{
		m_flyFrame->Update();
		auto cmdAllocator = m_flyFrame->GetCurrentFrame().CommandAllocator.Get();

		m_renderDevice->PrepareForRenderCommands(cmdAllocator, nullptr);

		m_renderDevice->SetViewport(1, &m_viewport);

		m_renderDevice->SetScissorRect(1, &m_rect);

		m_renderTexture->Begin();

		float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };

		m_renderTexture->Clear(clearColor, 1.0f, 0.0);
	}

	void D3DRenderer::EndRender()
	{
		m_renderTexture->End();

		m_renderDevice->ExecuteCommands();

		m_renderDevice->PresentRender();

		m_renderDevice->UpdateFenceValueBy(1);

		m_flyFrame->GetCurrentFrame().FenceValue = m_renderDevice->GetFenceValue();

		m_renderDevice->WaitForGPU();
	}

	void D3DRenderer::Resize()
	{
		m_renderTexture->Resize(m_window->GetWidth(), m_window->GetHeight());

		m_viewport = { 0,0, static_cast<FLOAT>(m_window->GetWidth()), static_cast<FLOAT>(m_window->GetHeight()) };
		m_rect = { 0,0, static_cast<LONG>(m_window->GetWidth()),static_cast<LONG>(m_window->GetHeight()) };
	}

	void D3DRenderer::Close()
	{
		m_renderDevice->Close();
	}
}
