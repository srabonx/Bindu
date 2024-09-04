#include "RenderTexture.h"

#include "D3DUtillity.h"
#include "d3dx12.h"
#include "../../../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	RenderTexture::RenderTexture(DXGI_FORMAT rtBuffFormat, DXGI_FORMAT dsBuffFormat, std::uint8_t bufferCount) :
		m_bufferCount(bufferCount), m_rtBuffFormat(rtBuffFormat), m_dsBuffFormat(dsBuffFormat)
	{
		
	}

	RenderTexture::~RenderTexture()
	{
		
	}

	void RenderTexture::Initialize(ID3D12Device* pDevice, CpuDescriptorHeap* pRtvHeap, CpuDescriptorHeap* pDsvHeap, CpuDescriptorHeap* pSrvHeap)
	{

		if (!pDevice)
			THROW_EXCEPTION(3, "Invalid D3D12Device");

		if (!pRtvHeap || !pDsvHeap || !pSrvHeap)
			THROW_EXCEPTION(3, "Invalid Descriptor Heap");

		if (m_bufferCount <= 0)
			THROW_EXCEPTION(3, "Invalid buffer count");

		// Check if the device supports requested format
		D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = { m_rtBuffFormat, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };


		DXThrowIfFailed(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport)));

		UINT requiredFormats = D3D12_FORMAT_SUPPORT1_RENDER_TARGET | D3D12_FORMAT_SUPPORT1_TEXTURE2D;

		if ((formatSupport.Support1 & requiredFormats) != requiredFormats)
			THROW_EXCEPTION(3, "Direct3D device does not support required format");

		// Allocate space in the rtv heap
		m_rtvCpuHeapAllocation = pRtvHeap->Allocate(m_bufferCount);

		// Allocate space in the dsv heap
		m_dsvCpuHeapAllocation = pDsvHeap->Allocate(1);

		// Allocate space in the srv heap
		m_srvCpuHeapAllocation = pSrvHeap->Allocate(m_bufferCount);

		m_d3dDevice = pDevice;
	}


	void RenderTexture::Begin()
	{
		
	}

	void RenderTexture::End()
	{
		
	}

	void RenderTexture::Resize(std::uint16_t width, std::uint16_t height)
	{
		// If the width and height are same then no need to do anything
		if (m_width == width && m_height == height)
			return;

		// If no device has been set
		if (!m_d3dDevice)
			return;

		// Reset the previous resource
		Reset();

		
		
		
	}

	void RenderTexture::Reset()
	{
		for (auto& r : m_rtBuffers)
			r.Reset();

		m_dsBuffer.Reset();

		m_currentState = D3D12_RESOURCE_STATE_RENDER_TARGET;

		m_width = m_height = 0;
	}

	ID3D12Resource* RenderTexture::GetCurrentRtBuffer() const
	{
	}

	ID3D12Resource* RenderTexture::GetDsBuffer() const
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetCurrentRtvCpuHandle() const
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetCurrentSrvCpuHandle() const
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetDsvCpuHandle() const
	{
	}

	void RenderTexture::TransitionTo(D3D12_RESOURCE_STATES state)
	{

	}

	void RenderTexture::CreateRTBuffer(std::uint16_t width, std::uint16_t height)
	{
		if (!m_d3dDevice)
			return;

		// Create the rt buffers
		CD3DX12_HEAP_PROPERTIES	heapProp(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_HEAP_FLAGS heapFlag = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

		D3D12_RESOURCE_DESC rsrcDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_rtBuffFormat,
			width,
			height,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// TODO: Implement Vec4 implementation
		float color[4] = { 1,0,0,1 };

		CD3DX12_CLEAR_VALUE clearVal(m_rtBuffFormat, color);

		// Create commited resources
		for (auto& i : m_rtBuffers)
		{
			DXThrowIfFailed(
				m_d3dDevice->CreateCommittedResource(&heapProp, heapFlag, &rsrcDesc,
					m_currentState, &clearVal, IID_PPV_ARGS(i.ReleaseAndGetAddressOf())));

		}

		// Create RTV and SRV on the rt buffer resources
		for (auto itr = m_rtBuffers.begin(); itr != m_rtBuffers.end(); ++itr)
		{
			auto index = std::distance(m_rtBuffers.begin(), itr);

			m_d3dDevice->CreateRenderTargetView(itr->Get(), nullptr,
				m_rtvCpuHeapAllocation.GetCpuHandle(static_cast<std::uint32_t>(index)));

			m_d3dDevice->CreateShaderResourceView(itr->Get(), nullptr,
				m_srvCpuHeapAllocation.GetCpuHandle(static_cast<std::uint32_t>(index)));
		}

	}

	void RenderTexture::CreateDSBuffer(std::uint16_t width, std::uint16_t height)
	{
		if (!m_d3dDevice)
			return;

		// Create depth stencil buffer

		D3D12_RESOURCE_DESC rsrcDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_dsBuffFormat,
			width, height,
			1, 1, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_HEAP_FLAGS heapFlag = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;

		CD3DX12_CLEAR_VALUE	clearValue(m_dsBuffFormat, 1.0f, 0.0f);

		// Create the resource
		DXThrowIfFailed(
			m_d3dDevice->CreateCommittedResource(&heapProp, heapFlag, &rsrcDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(m_dsBuffer.ReleaseAndGetAddressOf())));

		// Create DSV on the resource
		m_d3dDevice->CreateDepthStencilView(m_dsBuffer.Get(), nullptr, m_dsvCpuHeapAllocation.GetCpuHandle());
	}
}
