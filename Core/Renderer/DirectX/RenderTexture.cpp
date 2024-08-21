#include "RenderTexture.h"

#include <wrl/client.h>

#include "../../../Utility/Common/CommonUtility.h"
#include "Utility/D3DUtillity.h"
#include "Utility/d3dx12.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	class RenderTexture::Impl
	{
	public:
		ComPtr<ID3D12Device>	m_d3dDevice{ nullptr };
		ComPtr<ID3D12Resource>	m_resource{ nullptr };

		D3D12_CPU_DESCRIPTOR_HANDLE	m_srvDescriptor;
		D3D12_CPU_DESCRIPTOR_HANDLE	m_rtvDescriptor;

		D3D12_RESOURCE_STATES	m_currState{ D3D12_RESOURCE_STATE_COMMON };

		float					m_clearColor[4];

		DXGI_FORMAT				m_format;

		size_t					m_width{ 0 };
		size_t					m_height{ 0 };

	};



	RenderTexture::RenderTexture(DXGI_FORMAT format) : m_impl(std::make_unique<Impl>())
	{
		m_impl->m_format = format;
	}

	RenderTexture::~RenderTexture()
	{
	}

	void RenderTexture::SetDevice(ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor)
	{
		if (m_impl->m_d3dDevice.Get() == pDevice &&
			m_impl->m_srvDescriptor.ptr == srvDescriptor.ptr &&
			m_impl->m_rtvDescriptor.ptr == rtvDescriptor.ptr)
			return;

		if (m_impl->m_d3dDevice)
			ReleaseDevice();

		D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = { m_impl->m_format, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };

		if(FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT,&formatSupport,sizeof(formatSupport))))
		{
			THROW_EXCEPTION(3, "Check feature support");
		}

		UINT required = D3D12_FORMAT_SUPPORT1_RENDER_TARGET | D3D12_FORMAT_SUPPORT1_TEXTURE2D;

		if((formatSupport.Support1 & required) != required)
		{
			THROW_EXCEPTION(3, "D3D device does not support requested format");
		}

		if(!srvDescriptor.ptr || !rtvDescriptor.ptr)
		{
			THROW_EXCEPTION(3, "Invalid descriptors");
		}

		m_impl->m_d3dDevice = pDevice;
		m_impl->m_srvDescriptor = srvDescriptor;
		m_impl->m_rtvDescriptor = rtvDescriptor;
	}

	void RenderTexture::SizeResources(size_t width, size_t height, ID3D12GraphicsCommandList* pCommandList)
	{
		if (m_impl->m_width == width && m_impl->m_height == height)
			return;

		if (m_impl->m_width > UINT32_MAX || m_impl->m_height > UINT32_MAX)
			THROW_EXCEPTION(3, "Invalid width/height");

		if (!m_impl->m_d3dDevice)
			return;

		m_impl->m_width = m_impl->m_height = 0;

		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_RESOURCE_DESC rsd = CD3DX12_RESOURCE_DESC::Tex2D(
			m_impl->m_format,
			static_cast<UINT64>(width),
			static_cast<UINT>(height),
			1, 1, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		
		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = m_impl->m_format;
		memcpy(&clearValue.Color, m_impl->m_clearColor, sizeof(clearValue.Color));

		m_impl->m_currState = D3D12_RESOURCE_STATE_RENDER_TARGET;

		// Create Render Target

		DXThrowIfFailed(m_impl->m_d3dDevice->CreateCommittedResource(&heapProperties,
			D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
			&rsd, m_impl->m_currState, &clearValue, IID_PPV_ARGS(m_impl->m_resource.ReleaseAndGetAddressOf())));


		// Create RTV
		m_impl->m_d3dDevice->CreateRenderTargetView(m_impl->m_resource.Get(), nullptr, m_impl->m_rtvDescriptor);


		// Create SRV
		m_impl->m_d3dDevice->CreateShaderResourceView(m_impl->m_resource.Get(), nullptr, m_impl->m_srvDescriptor);

		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_impl->m_resource.Get(), m_impl->m_currState,
			D3D12_RESOURCE_STATE_PRESENT);

		pCommandList->ResourceBarrier(1, &transition);

		m_impl->m_currState = D3D12_RESOURCE_STATE_PRESENT;

		m_impl->m_width = width;
		m_impl->m_height = height;

	}

	void RenderTexture::ReleaseDevice()
	{
		m_impl->m_resource.Reset();

		m_impl->m_d3dDevice.Reset();

		m_impl->m_currState = D3D12_RESOURCE_STATE_COMMON;

		m_impl->m_width = m_impl->m_height = 0;

		m_impl->m_srvDescriptor.ptr = m_impl->m_rtvDescriptor.ptr = 0;
	}

	void RenderTexture::TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES stateAfter)
	{
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_impl->m_resource.Get(), m_impl->m_currState, stateAfter);

		commandList->ResourceBarrier(1, &transition);

		m_impl->m_currState = stateAfter;
	}

	void RenderTexture::BeginScene(ID3D12GraphicsCommandList* commandList)
	{
		TransitionTo(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	void RenderTexture::Clear(ID3D12GraphicsCommandList* commandList)
	{
		commandList->ClearRenderTargetView(m_impl->m_rtvDescriptor, m_impl->m_clearColor, 0, nullptr);
	}

	void RenderTexture::EndScene(ID3D12GraphicsCommandList* commandList)
	{
		TransitionTo(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void RenderTexture::SetClearColor(DirectX::FXMVECTOR color)
	{
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(m_impl->m_clearColor), color);
	}

	ComPtr<ID3D12Resource>& RenderTexture::GetResource()
	{
		return m_impl->m_resource;
	}
	DXGI_FORMAT RenderTexture::GetFormat() const
	{
		return m_impl->m_format;
	}

	size_t RenderTexture::GetHeight() const
	{
		return m_impl->m_height;
	}

	size_t RenderTexture::GetWidth() const
	{
		return m_impl->m_width;
	}
}
