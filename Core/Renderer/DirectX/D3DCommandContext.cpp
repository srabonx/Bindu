#include "D3DCommandContext.h"

#include "D3DFence.h"
#include "D3DDeviceManager.h"
#include "D3DUtillity.h"
#include "../../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	D3DCommandContext::D3DCommandContext()
	{
	}
	D3DCommandContext::~D3DCommandContext()
	{
	}

	void D3DCommandContext::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager)
	{
		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		CreateCommandObjects(deviceManager.get());

		m_deviceManager = deviceManager;
		m_commandQueue = deviceManager->GetCommandQueue();
	}

	void D3DCommandContext::PrepareForCommands(ID3D12CommandAllocator* commandAllocator) const
	{
		auto cmdAllocator = commandAllocator;

		if (cmdAllocator == nullptr)
			cmdAllocator = m_commandAllocator.Get();

		DXThrowIfFailed(
			cmdAllocator->Reset());

		DXThrowIfFailed(
			m_commandList->Reset(cmdAllocator, nullptr));

	}

	void D3DCommandContext::ExecuteCommands() const
	{
		DXThrowIfFailed(
			m_commandList->Close());

		ID3D12CommandList* commandList[] = {m_commandList.Get()};

		m_commandQueue->ExecuteCommandLists(_countof(commandList), commandList);
	}

	void D3DCommandContext::Signal(const D3DFence* fence) const
	{
		DXThrowIfFailed(
			m_commandQueue->Signal(fence->m_fence.Get(), fence->m_fenceValue));
	}

	void D3DCommandContext::WaitForGpu(const D3DFence* fence) const
	{
		// Signal to mark this point
		Signal(fence);

		// If GPU haven't reached this point
		if(fence->m_fence->GetCompletedValue() < fence->m_fenceValue)
		{
			// Create an event
			HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

			// Command to Fire event when GPU reaches this fence point
			fence->m_fence->SetEventOnCompletion(fence->m_fenceValue, eventHandle);

			// Wait for the event to be fired
			WaitForSingleObject(eventHandle, INFINITE);

			// Close the event handle
			CloseHandle(eventHandle);
		}

	}

	D3DDeviceManager* D3DCommandContext::GetDeviceManager() const
	{
		return m_deviceManager.get();
	}


	ComPtr<ID3D12GraphicsCommandList> D3DCommandContext::GetCommandList() const
	{
		return m_commandList;
	}

	ComPtr<ID3D12CommandAllocator> D3DCommandContext::GetCommandAllocator() const
	{
		return m_commandAllocator;
	}

	ComPtr<ID3D12CommandQueue> D3DCommandContext::GetCommandQueue() const
	{
		return m_commandQueue;
	}


	void D3DCommandContext::CreateCommandObjects(const D3DDeviceManager* deviceManager)
	{
		auto d3dDevice = deviceManager->GetD3DDevice();

		// Create CommandAllocator
		DXThrowIfFailed(
			d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf())));
		
		// Create CommandList
		DXThrowIfFailed(
			d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(),
				nullptr, IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf())));

		//// Create CommandQueue
		//D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		//commandQueueDesc.NodeMask = 0;
		//commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		//commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		//DXThrowIfFailed(
		//	d3dDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf())));
	}
}
