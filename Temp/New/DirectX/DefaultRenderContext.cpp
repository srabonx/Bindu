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

		m_depthStencilBuffer = std::make_unique<DepthStencilBuffer>(1.0f, 0);

		m_flyFrame = std::make_unique<FlyFrame>(3,2, maxObjectsToRender);

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

		ResourceProperties dsbProp = {};
		dsbProp.InitialState = D3D12_RESOURCE_STATE_COMMON;
		dsbProp.HeapFlags = D3D12_HEAP_FLAG_NONE;
		dsbProp.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		dsbProp.ResourceFlags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		dsbProp.ResourceFormat = DXGI_FORMAT_R24G8_TYPELESS;
		dsbProp.Width = m_width;
		dsbProp.Height = m_height;

		D3D12_CLEAR_VALUE	clearValue = {};
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		clearValue.DepthStencil.Depth = m_depthStencilBuffer->GetDepth();
		clearValue.DepthStencil.Stencil = m_depthStencilBuffer->GetStencil();

		dsbProp.OptimizedClearValue = &clearValue;
		m_depthStencilBuffer->Initialize(dsbProp, m_deviceManager);
		m_depthStencilBuffer->CreateDepthStencilView(DXGI_FORMAT_D24_UNORM_S8_UINT);
		m_depthStencilBuffer->TransitionTo(*m_commandContext, D3D12_RESOURCE_STATE_DEPTH_WRITE);

		m_commandContext->ExecuteCommands();

		m_fence->Advance();

		m_commandContext->WaitForGpu(m_fence.get());

	}

	void DefaultRenderContext::BeginRender(float clearColor[4], RenderTexture* renderTexture)
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

		RenderTexture* rt{ nullptr };

		if (renderTexture == nullptr)
			rt = m_swapChain->GetRenderTarget();
		else
			rt = renderTexture;

		rt->SetClearColor(clearColor);

		m_commandContext->Begin(rt, m_depthStencilBuffer.get());
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

		m_commandContext->End();

		m_commandContext->ExecuteCommands();

		m_swapChain->PresentRender(0,0);

		m_flyFrame->GetCurrentFrame()->FenceValue = m_fence->Advance();

		m_commandContext->Signal(m_fence.get());

		m_deviceManager->GetCbvSrvUavGpuHeap()->ReleaseStaleAllocations(m_fence->GetCurrentValue());

	//	m_commandContext->WaitForGpu(m_fence.get());
	}

	void DefaultRenderContext::UpdateScene(double dt, Scene* scene) const
	{
		scene->Update(dt, m_flyFrame->GetCurrentFrame());
	}

	void DefaultRenderContext::UpdatePerPassConstants(std::uint8_t passIndex, const PerPassConstants& perPassConstants)
	{
		m_perPassConstants = perPassConstants;

		auto currFrameCb = m_flyFrame->GetCurrentFrame()->PerPassCb.get();

		currFrameCb->CopyData(passIndex, &m_perPassConstants);
	}

	void DefaultRenderContext::Resize(std::uint16_t width, std::uint16_t height)
	{
		m_fence->Advance();
		m_commandContext->WaitForGpu(m_fence.get());

		m_swapChain->Resize(width, height);

		m_depthStencilBuffer->Resize(width, height);

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

	DepthStencilBuffer* DefaultRenderContext::GetDepthStencilBuffer() const
	{
		return m_depthStencilBuffer.get();
	}
}
