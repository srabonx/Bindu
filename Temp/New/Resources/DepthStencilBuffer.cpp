#include "DepthStencilBuffer.h"

#include "../Renderer/DirectX/CpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"

namespace BINDU
{
	DepthStencilBuffer::DepthStencilBuffer(float depth, std::uint8_t stencil) : m_depth(depth), m_stencil(stencil)
	{
	}

	float DepthStencilBuffer::GetDepth() const
	{
		return m_depth;
	}

	std::uint8_t DepthStencilBuffer::GetStencil() const
	{
		return m_stencil;
	}

	DescriptorHeapAllocation* DepthStencilBuffer::GetDsvAllocation()
	{
		return &m_dsvAllocation;
	}

	DescriptorHeapAllocation* DepthStencilBuffer::GetSrvAllocation()
	{
		return &m_srvAllocation;
	}

	void DepthStencilBuffer::CreateDepthStencilView(DXGI_FORMAT depthStencilFormat)
	{
		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();

		m_dsvAllocation = m_parentDeviceManager->GetDsvCpuHeap()->Allocate(1);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = depthStencilFormat;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		d3dDevice->CreateDepthStencilView(m_resource.Get(), &dsvDesc, m_dsvAllocation.GetCpuHandle());
	}

	void DepthStencilBuffer::CreateShaderResourceView()
	{
	}

	void DepthStencilBuffer::Resize(std::uint16_t width, std::uint16_t height)
	{
		Resource::Resize(width, height);

		CreateDepthStencilView(m_depthStencilFormat);
	}

	DXGI_FORMAT DepthStencilBuffer::GetDepthStencilFormat() const
	{
		return m_depthStencilFormat;
	}
}
