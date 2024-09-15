#include "D3DFence.h"

#include "D3DRenderDevice.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "Utility/D3DUtillity.h"

namespace BINDU
{
	void D3DFence::Initialize(const std::shared_ptr<D3DRenderDevice>& parentDevice)
	{
		if (!parentDevice)
			THROW_EXCEPTION(3, "Invalid Device");

		auto d3dDevice = parentDevice->GetD3DDevice();

		DXThrowIfFailed(
			d3dDevice->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));

		m_parentDevice = parentDevice;
	}

	std::uint64_t D3DFence::Advance()
	{
		return ++m_fenceValue;
	}
	D3DRenderDevice* D3DFence::GetParentDevice() const
	{
		return m_parentDevice.get();
	}

	std::uint64_t D3DFence::GetCurrentValue() const
	{
		return m_fenceValue;
	}
}
