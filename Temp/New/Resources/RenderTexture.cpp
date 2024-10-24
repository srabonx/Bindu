#include "RenderTexture.h"

#include "../Renderer/DirectX/GpuDescriptorHeap.h"
#include "../Renderer/DirectX/CpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"

namespace BINDU
{
	RenderTexture::RenderTexture(ComPtr<ID3D12Resource>& resource)
	{
		m_resource = resource;
		m_hasExternalResource = true;
	}

	void RenderTexture::Initialize(const ResourceProperties& properties,
		const std::shared_ptr<D3DDeviceManager>& parentDevice)
	{
		if (!m_hasExternalResource)
			Resource::Initialize(properties, parentDevice);
		else
			m_parentDeviceManager = parentDevice;
	}

	void RenderTexture::CreateRenderTargetView(DXGI_FORMAT renderTargetFormat)
	{
		m_renderTargetFormat = renderTargetFormat;

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = renderTargetFormat;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();

		m_rtvAllocation = m_parentDeviceManager->GetRtvCpuHeap()->Allocate(1);

		d3dDevice->CreateRenderTargetView(m_resource.Get(), &rtvDesc, m_rtvAllocation.GetCpuHandle());
	}

	void RenderTexture::CreateShaderResourceView(DXGI_FORMAT srvFormat)
	{
		m_srvFormat = srvFormat;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = srvFormat;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();

		m_srvAllocation = m_parentDeviceManager->GetCbvSrvUavGpuHeap()->Allocate(1);

		d3dDevice->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srvAllocation.GetCpuHandle());

		m_hasSrv = true;
	}

	void RenderTexture::Resize(std::uint16_t width, std::uint16_t height)
	{
		if (!m_hasExternalResource)
			Resource::Resize(width, height);

		CreateRenderTargetView(m_renderTargetFormat);

		if (m_hasSrv)
			CreateShaderResourceView(m_srvFormat);
	}

	void RenderTexture::SetClearColor(float clearColor[4])
	{
		m_clearColor[0] = clearColor[0];
		m_clearColor[1] = clearColor[1];
		m_clearColor[2] = clearColor[2];
		m_clearColor[3] = clearColor[3];
	}

	float* RenderTexture::GetClearColor()
	{
		return m_clearColor;
	}

	DescriptorHeapAllocation* RenderTexture::GetRtvAllocation()
	{
		return &m_rtvAllocation;
	}

	DescriptorHeapAllocation* RenderTexture::GetSrvAllocation()
	{
		return &m_srvAllocation;
	}
}
