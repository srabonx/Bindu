#include "RenderTexture.h"

#include "D3DUtillity.h"
#include "d3dx12.h"
#include "../../../Utility/Common/CommonUtility.h"

#include "D3DDeviceManager.h"
#include "D3DCommandContext.h"

namespace BINDU
{
	RenderTexture::RenderTexture(DXGI_FORMAT rtBuffFormat, DXGI_FORMAT dsBuffFormat, std::uint8_t bufferCount, DXGI_SAMPLE_DESC sampleDesc) :
		m_bufferCount(bufferCount), m_rtBuffFormat(rtBuffFormat), m_dsBuffFormat(dsBuffFormat),
		m_sampleDesc(sampleDesc),
		m_currentState(D3D12_RESOURCE_STATE_COMMON)
	{
		m_rtBuffers.resize(bufferCount);
	}

	RenderTexture::~RenderTexture()
	{

	}

	void RenderTexture::Initialize(std::uint16_t width, std::uint16_t height, const std::shared_ptr<D3DDeviceManager>& deviceManager)
	{

		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		if (m_bufferCount <= 0)
			THROW_EXCEPTION(3, "Invalid buffer count");

		m_deviceManager = deviceManager;
		m_width = width;
		m_height = height;

		CheckFeatureSupport(deviceManager.get());

		// Allocate space in the rtv heap
		m_rtvCpuHeapAllocation = deviceManager->GetRtvCpuHeap()->Allocate(m_bufferCount);

		// Allocate space in the dsv heap
		m_dsvCpuHeapAllocation = deviceManager->GetDsvCpuHeap()->Allocate(1);

		// Allocate space in the srv heap
		m_srvCpuHeapAllocation = deviceManager->GetCbvSrvUavCpuHeap()->Allocate(m_bufferCount);


		CreateRTBuffer(width, height);
		CreateDSBuffer(width, height);

	}

	void RenderTexture::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager, const std::vector<ComPtr<ID3D12Resource>>& renderTargetBuffers)
	{
		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");

		if(m_bufferCount != renderTargetBuffers.size())
			THROW_EXCEPTION(3, "Buffer count mismatch");

		auto bufferDesc = renderTargetBuffers[0]->GetDesc();

		m_rtBuffers.clear();
		m_rtBuffers = renderTargetBuffers;
		m_deviceManager = deviceManager;
		m_width = static_cast<std::uint16_t>(bufferDesc.Width);
		m_height = static_cast<std::uint16_t>(bufferDesc.Height);

		CheckFeatureSupport(deviceManager.get());

		// Allocate space in the rtv heap
		m_rtvCpuHeapAllocation = deviceManager->GetRtvCpuHeap()->Allocate(m_bufferCount);

		// Allocate space in the dsv heap
		m_dsvCpuHeapAllocation = deviceManager->GetDsvCpuHeap()->Allocate(1);

		// Allocate space in the srv heap
		m_srvCpuHeapAllocation = deviceManager->GetCbvSrvUavCpuHeap()->Allocate(m_bufferCount);


		CreateRTV(nullptr);
		CreateSRV(nullptr);

		CreateDSBuffer(m_width, m_height);
	}

	void RenderTexture::Resize(std::uint16_t width, std::uint16_t height)
	{
		// If the width and height are same then no need to do anything
		if (m_width == width && m_height == height)
			return;

		// Reset the previous resource
		Reset();

		m_currentRtBuffer = 0;

		CreateRTBuffer(width, height);

		CreateRTV(nullptr);
		CreateSRV(nullptr);

		CreateDSBuffer(width, height);

		m_width = width;

		m_height = height;

	}

	void RenderTexture::Reset()
	{
		for (auto& r : m_rtBuffers)
			r.Reset();

		m_dsBuffer.Reset();

		m_currentState = D3D12_RESOURCE_STATE_COMMON;

		m_width = m_height = 0;
	}

	ID3D12Resource* RenderTexture::GetRtBuffer(int index) const
	{
		if (index < 0 || index >= m_bufferCount)
			return nullptr;

		return m_rtBuffers[index].Get();
	}

	ID3D12Resource* RenderTexture::GetCurrentRtBuffer() const
	{
		return m_rtBuffers[m_currentRtBuffer].Get();
	}

	ID3D12Resource* RenderTexture::GetDsBuffer() const
	{
		return m_dsBuffer.Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetCurrentRtvCpuHandle() const
	{
		return m_rtvCpuHeapAllocation.GetCpuHandle(m_currentRtBuffer);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetCurrentSrvCpuHandle() const
	{
		return m_srvCpuHeapAllocation.GetCpuHandle(m_currentRtBuffer);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetRtvCpuHandle(int index) const
	{
		if (index < 0 || index >= m_bufferCount)
			return {};

		return m_rtvCpuHeapAllocation.GetCpuHandle(index);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetSrvCpuHandle(int index) const
	{
		if (index < 0 || index >= m_bufferCount)
			return {};

		return m_srvCpuHeapAllocation.GetCpuHandle(index);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetDsvCpuHandle() const
	{
		return m_dsvCpuHeapAllocation.GetCpuHandle();
	}

	std::uint8_t RenderTexture::GetBufferCount() const
	{
		return m_bufferCount;
	}

	DXGI_FORMAT RenderTexture::GetRenderTargetBufferFormat() const
	{
		return m_rtBuffFormat;
	}

	DXGI_FORMAT RenderTexture::GetDepthStencilBufferFormat() const
	{
		return m_dsBuffFormat;
	}

	DXGI_SAMPLE_DESC RenderTexture::GetSampleDesc() const
	{
		return m_sampleDesc;
	}

	std::uint16_t RenderTexture::GetWidth() const
	{
		return m_width;
	}

	std::uint16_t RenderTexture::GetHeight() const
	{
		return m_height;
	}

	const float* RenderTexture::GetClearColor() const
	{
		return m_clearColor;
	}

	float RenderTexture::GetDepth() const
	{
		return m_depth;
	}

	std::uint8_t RenderTexture::GetStencil() const
	{
		return m_stencil;
	}

	void RenderTexture::SetClearColor(float clearColor[4])
	{
		m_clearColor = { clearColor };
	}

	void RenderTexture::SetDepth(float depth)
	{
		m_depth = depth;
	}

	void RenderTexture::SetStencil(std::uint8_t stencil)
	{
		m_stencil = stencil;
	}

	void RenderTexture::SetDepthStencil(float depth, std::uint8_t stencil)
	{
		m_depth = depth;

		m_stencil = stencil;
	}

	void RenderTexture::SetSampleDesc(DXGI_SAMPLE_DESC sampleDesc)
	{
		m_sampleDesc = sampleDesc;

		// Recreate the RT buffer and DS buffer

		// Reset() resets the width and height value, So caching it here
		auto width = m_width;

		auto height = m_height;

		Reset();

		Resize(width, height);
	}

	void RenderTexture::SetState(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& state)
	{
		TransitionTo(commandList, state);
	}


	void RenderTexture::TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state)
	{
		auto resource = m_rtBuffers[m_currentRtBuffer].Get();
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(resource, m_currentState, state);
		commandList->ResourceBarrier(1, &transition);

		m_currentState = state;

		if (state == D3D12_RESOURCE_STATE_PRESENT)
			m_currentRtBuffer = (m_currentRtBuffer + 1) % m_bufferCount;
	}

	void RenderTexture::CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc)
	{
		auto d3dDevice = m_deviceManager->GetD3DDevice();
		// Create RTV and SRV on the rt buffer resources
		for (auto itr = m_rtBuffers.begin(); itr != m_rtBuffers.end(); ++itr)
		{
			auto index = std::distance(m_rtBuffers.begin(), itr);

			d3dDevice->CreateRenderTargetView(itr->Get(), rtvDesc,
				m_rtvCpuHeapAllocation.GetCpuHandle(static_cast<std::uint32_t>(index)));
		}
	}

	void RenderTexture::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc)
	{
		auto d3dDevice = m_deviceManager->GetD3DDevice();
		// Create RTV and SRV on the rt buffer resources
		for (auto itr = m_rtBuffers.begin(); itr != m_rtBuffers.end(); ++itr)
		{
			auto index = std::distance(m_rtBuffers.begin(), itr);

			d3dDevice->CreateShaderResourceView(itr->Get(), srvDesc,
				m_srvCpuHeapAllocation.GetCpuHandle(static_cast<std::uint32_t>(index)));
		}
	}



	void RenderTexture::CreateRTBuffer(std::uint16_t width, std::uint16_t height)
	{
		if (!m_deviceManager)
			return;

		auto d3dDevice = m_deviceManager->GetD3DDevice();

		// Create the rt buffers
		CD3DX12_HEAP_PROPERTIES	heapProp(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_HEAP_FLAGS heapFlag = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

		D3D12_RESOURCE_DESC rsrcDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_rtBuffFormat,
			width,
			height,
			1, 1, m_sampleDesc.Count, m_sampleDesc.Quality, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// TODO: Implement Vec4 implementation
		float color[4] = { 1,0,0,1 };

		CD3DX12_CLEAR_VALUE clearVal(m_rtBuffFormat, color);

		// Create commited resources

		for (auto& i : m_rtBuffers)
		{
			DXThrowIfFailed(
				d3dDevice->CreateCommittedResource(&heapProp, heapFlag, &rsrcDesc,
					D3D12_RESOURCE_STATE_COMMON, &clearVal, IID_PPV_ARGS(i.ReleaseAndGetAddressOf())));
		}

		CreateRTV(nullptr);
		CreateSRV(nullptr);

	}

	void RenderTexture::CreateDSBuffer(std::uint16_t width, std::uint16_t height)
	{
		if (!m_deviceManager)
			return;

		auto d3dDevice = m_deviceManager->GetD3DDevice();

		// Create depth stencil buffer

		D3D12_RESOURCE_DESC rsrcDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_dsBuffFormat,
			width, height,
			1, 1, m_sampleDesc.Count, m_sampleDesc.Quality,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_HEAP_FLAGS heapFlag = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

		CD3DX12_CLEAR_VALUE	clearValue(m_dsBuffFormat, 1.0f, 0.0f);

		// Create the resource
		DXThrowIfFailed(
			d3dDevice->CreateCommittedResource(&heapProp, heapFlag, &rsrcDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(m_dsBuffer.ReleaseAndGetAddressOf())));

		// Create DSV on the resource
		d3dDevice->CreateDepthStencilView(m_dsBuffer.Get(), nullptr, m_dsvCpuHeapAllocation.GetCpuHandle());
	}

	void RenderTexture::CheckFeatureSupport(const D3DDeviceManager* deviceManager) const
	{
		auto d3dDevice = deviceManager->GetD3DDevice();

		// Check if the device supports requested format
		D3D12_FEATURE_DATA_FORMAT_SUPPORT rtFormatSupport = { m_rtBuffFormat, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };

		D3D12_FEATURE_DATA_FORMAT_SUPPORT dsFormatSupport = { m_dsBuffFormat, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };

		DXThrowIfFailed(d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &rtFormatSupport, sizeof(rtFormatSupport)));

		DXThrowIfFailed(d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &dsFormatSupport, sizeof(dsFormatSupport)));


		UINT rtRequiredFormats = D3D12_FORMAT_SUPPORT1_RENDER_TARGET | D3D12_FORMAT_SUPPORT1_TEXTURE2D;

		UINT dsRequiredFormats = D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL | D3D12_FORMAT_SUPPORT1_TEXTURE2D;

		if ((rtFormatSupport.Support1 & rtRequiredFormats) != rtRequiredFormats)
			THROW_EXCEPTION(3, "Direct3D device does not support required format");

		if ((dsFormatSupport.Support1 & dsRequiredFormats) != dsRequiredFormats)
			THROW_EXCEPTION(3, "Direct3D device does not support required format");
	}
}
