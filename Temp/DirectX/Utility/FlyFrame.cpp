#include "FlyFrame.h"

#include "D3DUtillity.h"
#include "../D3DRenderDevice.h"

namespace BINDU
{
	FlyFrame::FlyFrame(std::uint8_t count) : m_frameCount(count)
	{
		m_frames.resize(count);
	}

	FlyFrame::~FlyFrame()
	{
	}

	void FlyFrame::Initialize(const std::shared_ptr<D3DRenderDevice>& pRenderDevice)
	{
		if (!pRenderDevice)
			return;

		for(auto& frame : m_frames)
		{
			DXThrowIfFailed(
				pRenderDevice->GetD3DDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(frame.CommandAllocator.ReleaseAndGetAddressOf())));
		}

		m_passCBV = pRenderDevice->GetCbvSrvUavGpuHeap()->Allocate(m_frameCount);

		m_parentDevice = pRenderDevice;

		m_initialized = true;
	}

	void FlyFrame::Update()
	{
		m_currentFrame = (m_currentFrame + 1) % m_frameCount;
	}

	FrameResource& FlyFrame::GetCurrentFrame()
	{
		return m_frames[m_currentFrame];
	}

	D3DRenderDevice* FlyFrame::GetParentDevice() const
	{
		return m_parentDevice.get();
	}

	std::uint16_t FlyFrame::GetFrameCount() const
	{
		return m_frameCount;
	}

	FrameResource& FlyFrame::GetFrame(std::uint16_t index)
	{
		if (index < 0 || index >= m_frameCount)
			THROW_EXCEPTION(3, "Invalid index");

		return m_frames[index];
	}

	DescriptorHeapAllocation* FlyFrame::GetGPUHeapAllocation() 
	{
		return &m_passCBV;
	}
}
