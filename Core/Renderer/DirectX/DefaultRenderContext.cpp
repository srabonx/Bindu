#include "DefaultRenderContext.h"

#include "GpuDescriptorHeap.h"
#include "../../GameObject/GameObject.h"
#include "../../Scene/Scene.h"

namespace BINDU
{
	DefaultRenderContext::DefaultRenderContext(std::uint16_t width, std::uint16_t height, HWND hwnd, std::uint64_t maxObjectsToRender) :
			m_width(width), m_height(height), m_hwnd(hwnd)
	{
		m_deviceManager = std::make_shared<D3DDeviceManager>();
		m_commandContext = std::make_shared<D3DCommandContext>();
		m_swapChain = std::make_unique<D3DSwapChain>();
		m_fence = std::make_unique<D3DFence>();

		m_pipelineStateManager = std::make_unique<D3DPipelineStateManager>();

		m_flyFrame = std::make_unique<FlyFrame>(3, maxObjectsToRender);

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

		m_flyFrame->Update();

		auto currentFrame = m_flyFrame->GetCurrentFrame();

		if (currentFrame->FenceValue != 0)
			m_commandContext->WaitForGpu(m_fence.get(), currentFrame->FenceValue);

		auto cmdAllocator = currentFrame->CommandAllocator.Get();

		m_commandContext->PrepareForCommands(cmdAllocator);

		auto commandList = m_commandContext->GetCommandList();

		commandList->RSSetViewports(1, &m_viewport);

		commandList->RSSetScissorRects(1, &m_scissorRect);

		auto cbvSrvUavGpuHeap = m_deviceManager->GetCbvSrvUavGpuHeap()->GetDescriptorHeap()->Heap().Get();

		commandList->SetDescriptorHeaps(1, &cbvSrvUavGpuHeap);

		auto renderTexture = m_swapChain->GetRenderTarget();
		renderTexture->SetClearColor(clearColor);

		m_commandContext->Begin(renderTexture);
	}

	void DefaultRenderContext::BindPipeline(const std::string& pipelineName, const std::string& rootSigName) const
	{
		m_commandContext->BindPipeline(*m_pipelineStateManager, pipelineName, rootSigName);
	}

	void DefaultRenderContext::BindPipeline(ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature) const
	{
		m_commandContext->BindPipeline(pipelineState, rootSignature);
	}

	void DefaultRenderContext::BindPerPassConstant(std::uint16_t rootParamSlot) const
	{
		auto cmdList = m_commandContext->GetCommandList();

		cmdList->SetGraphicsRootConstantBufferView(rootParamSlot, m_flyFrame->GetCurrentFrame()->PerPassCb->GetGPUVirtualAddress());
	}

	void DefaultRenderContext::RenderScene(Scene* scene) const
	{
		scene->Render(*m_commandContext.get(), m_flyFrame->GetCurrentFrame());
	}

	void DefaultRenderContext::RenderGameObject(GameObject* gameObject) const
	{
		gameObject->Render(*m_commandContext, m_flyFrame->GetCurrentFrame()->ObjectsCb.get());
	}

	void DefaultRenderContext::EndRender()
	{

		auto currFrameCb = m_flyFrame->GetCurrentFrame()->PerPassCb.get();

		currFrameCb->CopyData(0, &m_perPassConstants);

		m_commandContext->End();

		m_commandContext->ExecuteCommands();

		m_swapChain->PresentRender(0,0);

		m_flyFrame->GetCurrentFrame()->FenceValue = m_fence->Advance();

		m_commandContext->Signal(m_fence.get());

	//	m_deviceManager->GetCbvSrvUavGpuHeap()->ReleaseStaleAllocations(m_fence->GetCurrentValue());

	//	m_commandContext->WaitForGpu(m_fence.get());
	}

	void DefaultRenderContext::UpdateScene(double dt, Scene* scene) const
	{
		scene->Update(dt, m_flyFrame->GetCurrentFrame());
	}

	void DefaultRenderContext::UpdatePerPassConstants(const PerPassConstants& perPassConstants)
	{
		m_perPassConstants = perPassConstants;
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

	std::shared_ptr<D3DDeviceManager> DefaultRenderContext::GetDeviceManager() const
	{
		return m_deviceManager;
	}

	std::shared_ptr<D3DCommandContext> DefaultRenderContext::GetCommandContext() const
	{
		return m_commandContext;
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
