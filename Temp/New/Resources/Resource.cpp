#include "Resource.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"
#include "../Renderer/DirectX/d3dx12.h"
#include "../Renderer/DirectX/D3DUtillity.h"

namespace BINDU
{

	void Resource::Initialize(const ResourceProperties& properties,
		const std::shared_ptr<D3DDeviceManager>& parentDevice)
	{
		m_flags = properties.ResourceFlags;
		m_format = properties.ResourceFormat;
		m_heapType = properties.HeapType;
		m_heapFlags = properties.HeapFlags;
		m_width = properties.Width;
		m_height = properties.Height;
		m_sampleDesc = properties.SampleDesc;
		m_currState = properties.InitialState;

		if(properties.OptimizedClearValue != nullptr)
		{
			m_optimizedClearValue = std::make_unique<D3D12_CLEAR_VALUE>();
			m_optimizedClearValue->Color[0] = properties.OptimizedClearValue->Color[0];
			m_optimizedClearValue->Color[1] = properties.OptimizedClearValue->Color[1];
			m_optimizedClearValue->Color[2] = properties.OptimizedClearValue->Color[2];
			m_optimizedClearValue->Color[3] = properties.OptimizedClearValue->Color[3];

			m_optimizedClearValue->Format = properties.OptimizedClearValue->Format;
			m_optimizedClearValue->DepthStencil = properties.OptimizedClearValue->DepthStencil;
		}

		m_parentDeviceManager = parentDevice;

		CreateCommitedResource(parentDevice->GetD3DDevice(), m_width, m_height);

	}

	void Resource::TransitionTo(const D3DCommandContext& commandContext, D3D12_RESOURCE_STATES newState)
	{
		if (m_currState == newState)
			return;
			
		auto cmdList = commandContext.GetCommandList();

		auto transition = CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), m_currState, newState);

		cmdList->ResourceBarrier(1, &transition);

		m_currState = newState;
	}

	void Resource::Reset()
	{
		m_resource.Reset();
	}

	void Resource::Resize(std::uint16_t width, std::uint16_t height)
	{
		if (m_width == width && m_height == height)
			return;

		Reset();

		auto d3dDevice = m_parentDeviceManager->GetD3DDevice();

		CreateCommitedResource(d3dDevice, width, height);

		m_width = width;
		m_height = height;
	}


	D3D12_RESOURCE_STATES Resource::GetState() const
	{
		return m_currState;
	}

	D3D12_RESOURCE_FLAGS Resource::GetFlags() const
	{
		return m_flags;
	}

	D3D12_RESOURCE_DESC Resource::GetDesc() const
	{
		return m_resource->GetDesc();
	}

	DXGI_FORMAT Resource::GetFormat() const
	{
		return m_format;
	}

	ComPtr<ID3D12Resource> Resource::GetResource() const
	{
		return m_resource;
	}

	void Resource::CreateCommitedResource(ID3D12Device* d3dDevice, std::uint16_t width, std::uint16_t height)
	{
		CD3DX12_HEAP_PROPERTIES heapProp(m_heapType);

		D3D12_HEAP_FLAGS l_heapFlags = m_heapFlags;

		D3D12_RESOURCE_DESC	rsd = CD3DX12_RESOURCE_DESC::Tex2D(m_format, width, height, 1, 1, m_sampleDesc.Count,
			m_sampleDesc.Quality, m_flags);

		DXThrowIfFailed(
			d3dDevice->CreateCommittedResource(&heapProp, l_heapFlags, &rsd, m_currState,
				m_optimizedClearValue.get(), IID_PPV_ARGS(m_resource.GetAddressOf())));
	}
}
