#include "FlyFrame.h"

#include "D3DDeviceManager.h"
#include "../../Managers/ConstantBufferManager.h"

namespace BINDU
{
	FlyFrame::FlyFrame(std::uint8_t frameCount, std::uint8_t passCount, std::uint64_t maxObjectsPerFrame) :
		m_frameCount(frameCount),m_passCount(passCount), m_maxObjects(maxObjectsPerFrame)
	{
		m_frames.resize(frameCount);
	}

	FlyFrame::~FlyFrame()
	{
	}

	void FlyFrame::Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager,
							std::uint32_t perPassConstantDataByteSize, std::uint32_t perObjectConstantDataByteSize)
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
			frame.PerPassCb->Initialize(d3dDevice, m_passCount, perPassConstantDataByteSize);

			frame.ObjectsCb = std::make_unique<UploadBuffer>(true);
			frame.ObjectsCb->Initialize(d3dDevice,m_maxObjects, perObjectConstantDataByteSize);

			frame.MaterialCb = std::make_unique<UploadBuffer>(true);
			frame.MaterialCb->Initialize(d3dDevice, 1000, sizeof(MaterialConstants));

			frame.LightCb = std::make_unique<UploadBuffer>(true);
			frame.LightCb->Initialize(d3dDevice, m_maxObjects, sizeof(LightConstants));
		}

		m_parentDeviceManager = parentDeviceManager;

		m_initialized = true;
	} 


	void FlyFrame::Update()
	{
		m_currentFrame = (m_currentFrame + 1) % m_frameCount;
	}

	void FlyFrame::UpdatePerPassCB(std::uint64_t cbIndex, void const* data)
	{
		GetCurrentFrame()->PerPassCb->CopyData(cbIndex, data);
	}

	void FlyFrame::UpdateObjectsCB(std::uint64_t cbIndex, void const* data)
	{
		GetCurrentFrame()->ObjectsCb->CopyData(cbIndex, data);
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
