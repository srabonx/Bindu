#include "DirectXRenderTexture.h"

#include "d3dx12.h"

namespace BINDU
{
	DirectXRenderTexture::DirectXRenderTexture(const TextureSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) :
		m_commandList(commandList), m_specification(specification), m_dsvHandle(dsvHandle)
	{
	}

	void DirectXRenderTexture::Bind(std::uint32_t slot)
	{
		if (m_isShaderVisible)
			m_commandList->SetGraphicsRootDescriptorTable(slot, m_srvAllocation.GetGpuHandle());

	}

	void DirectXRenderTexture::BindAsRt()
	{
		TransitionTo(D3D12_RESOURCE_STATE_RENDER_TARGET);

		auto rtvCpuHandle = m_rtvAllocation.GetCpuHandle();

		m_commandList->OMSetRenderTargets(1,
			&rtvCpuHandle, TRUE, &m_dsvHandle);
	}

	const TextureSpecification& DirectXRenderTexture::GetSpecification() const
	{
		return m_specification;
	}
	std::uint32_t DirectXRenderTexture::GetWidth()
	{
		return m_specification.Width;
	}
	std::uint32_t DirectXRenderTexture::GetHeight()
	{
		return m_specification.Height;
	}
	bool DirectXRenderTexture::IsLoaded()
	{
		return m_isLoaded;
	}

	bool DirectXRenderTexture::IsShaderVisible() const
	{
		return m_isShaderVisible;
	}

	DescriptorHeapAllocation& DirectXRenderTexture::GetRtvAllocation()
	{
		return m_rtvAllocation;
	}

	DescriptorHeapAllocation& DirectXRenderTexture::GetSrvAllocation()
	{
		return m_srvAllocation;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderTexture::GetDepthStencilViewHandle() const
	{
		return m_dsvHandle;
	}

	void DirectXRenderTexture::TransitionTo(D3D12_RESOURCE_STATES newState)
	{
		auto transition = CD3DX12_RESOURCE_BARRIER::Transition(m_d3dResource.Get(), m_currState, newState);
		m_commandList->ResourceBarrier(1, &transition);
		m_currState = newState;
	}

	ID3D12Resource* DirectXRenderTexture::GetResource() const
	{
		return m_d3dResource.Get();
	}
}
