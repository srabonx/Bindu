#include "FlyFrame.h"

#include "D3DDeviceManager.h"

namespace BINDU
{
	FlyFrame::FlyFrame(std::uint8_t count) : m_frameCount(count)
	{
		m_frames.resize(count);
	}

	FlyFrame::~FlyFrame()
	{
	}

	void FlyFrame::Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager)
	{
		if (!parentDeviceManager)
			return;

		auto d3dDevice = parentDeviceManager->GetD3DDevice();

		for (auto& frame : m_frames)
		{
			DXThrowIfFailed(
				d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(frame.CommandAllocator.ReleaseAndGetAddressOf())));

			frame.PerPassCb = std::make_unique<UploadBuffer>(true);
			frame.PerPassCb->Initialize<PerPassConstants>(d3dDevice, 1);
		}

		m_parentDeviceManager = parentDeviceManager;

		m_initialized = true;
	}


	void FlyFrame::Update()
	{
		m_currentFrame = (m_currentFrame + 1) % m_frameCount;
	}

	D3DDeviceManager* FlyFrame::GetParentDeviceManager() const
	{
		return m_parentDeviceManager.get();
	}

	FrameResource* FlyFrame::GetCurrentFrame()
	{
		return &m_frames[m_currentFrame];
	}


	std::uint16_t FlyFrame::GetFrameCount() const
	{
		return m_frameCount;
	}

	FrameResource* FlyFrame::GetFrame(std::uint16_t index)
	{
		if (index < 0 || index >= m_frameCount)
			THROW_EXCEPTION(3, "Invalid index");

		return &m_frames[index];
	}
}
