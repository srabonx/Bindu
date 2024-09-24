#include "D3DFence.h"

#include "D3DDeviceManager.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "D3DUtillity.h"

namespace BINDU
{
	void D3DFence::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager)
	{
		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		auto d3dDevice = deviceManager->GetD3DDevice();

		DXThrowIfFailed(
			d3dDevice->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));

		m_deviceManager = deviceManager;
	}

	std::uint64_t D3DFence::Advance()
	{
		return ++m_fenceValue;
	}

	std::uint64_t D3DFence::GetCurrentValue() const
	{
		return m_fenceValue;
	}

	std::uint64_t D3DFence::GetCompletedValue() const
	{
		return m_fence->GetCompletedValue();
	}
}
