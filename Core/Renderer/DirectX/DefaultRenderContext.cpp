#include "DefaultRenderContext.h"

#include "../../GameObject/GameObject.h"

namespace BINDU
{
	DefaultRenderContext::DefaultRenderContext(std::uint16_t width, std::uint16_t height, HWND hwnd): m_width(width), m_height(height), m_hwnd(hwnd)
	{
		m_deviceManager = std::make_shared<D3DDeviceManager>();
		m_commandContext = std::make_unique<D3DCommandContext>();
		m_swapChain = std::make_unique<D3DSwapChain>();
		m_fence = std::make_unique<D3DFence>();

		m_pipelineStateManager = std::make_unique<D3DPipelineStateManager>();

		m_viewport = D3D12_VIEWPORT{ 0,0,static_cast<float>(m_width),static_cast<float>(m_height), 0.0f,1.0f };
		m_scissorRect = D3D12_RECT{ 0,0, static_cast<LONG>(m_width),static_cast<LONG>(m_height) };
	}

	void DefaultRenderContext::Initialize()
	{
		m_deviceManager->Initialize();

		m_commandContext->Initialize(m_deviceManager);

		m_swapChain->Initialize(m_deviceManager, m_hwnd, m_width, m_height, 2);

		m_fence->Initialize(m_deviceManager);

		m_pipelineStateManager->Initialize(m_deviceManager);


		m_commandContext->ExecuteCommands();

		m_fence->Advance();

		m_commandContext->WaitForGpu(m_fence.get());

	}

	void DefaultRenderContext::BeginRender(float clearColor[4])
	{
		m_commandContext->PrepareForCommands(nullptr);

		auto commandList = m_commandContext->GetCommandList();

		commandList->RSSetViewports(1, &m_viewport);

		commandList->RSSetScissorRects(1, &m_scissorRect);

		auto renderTexture = m_swapChain->GetRenderTarget();
		renderTexture->SetClearColor(clearColor);

		m_commandContext->Begin(renderTexture);
	}

	void DefaultRenderContext::BindPipeline(const std::string& pipelineName) const
	{
		m_commandContext->BindPipeline(*m_pipelineStateManager, pipelineName);
	}

	void DefaultRenderContext::RenderGameObject(GameObject* gameObject) const
	{
		gameObject->Render(*m_commandContext);
	}

	void DefaultRenderContext::EndRender()
	{
		m_commandContext->End();

		m_commandContext->ExecuteCommands();

		m_swapChain->PresentRender(0,0);

		m_fence->Advance();

		m_commandContext->WaitForGpu(m_fence.get());
	}

	void DefaultRenderContext::Resize(std::uint16_t width, std::uint16_t height)
	{
		m_fence->Advance();
		m_commandContext->WaitForGpu(m_fence.get());

		m_swapChain->Resize(width, height);

		m_fence->Advance();
		m_commandContext->WaitForGpu(m_fence.get());

		m_width = width;
		m_height = height;

		m_viewport = D3D12_VIEWPORT{ 0,0,static_cast<float>(m_width),static_cast<float>(m_height), 0.0f,1.0f };
		m_scissorRect = D3D12_RECT{ 0,0, static_cast<LONG>(m_width),static_cast<LONG>(m_height) };
	}

	void DefaultRenderContext::Close() const
	{
		m_deviceManager->Close();
	}

	D3DPipelineStateManager* DefaultRenderContext::GetPipelineStateManager() const
	{
		return m_pipelineStateManager.get();
	}

	D3DDeviceManager* DefaultRenderContext::GetDeviceManager() const
	{
		return m_deviceManager.get();
	}

	D3DCommandContext* DefaultRenderContext::GetCommandContext() const
	{
		return m_commandContext.get();
	}

	D3DFence* DefaultRenderContext::GetFence() const
	{
		return m_fence.get();
	}

	D3DSwapChain* DefaultRenderContext::GetSwapChain() const
	{
		return m_swapChain.get();
	}
}
