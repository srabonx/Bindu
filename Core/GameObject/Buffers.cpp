#include "Buffers.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"
#include "../Renderer/DirectX/D3DUtillity.h"

namespace BINDU
{

	// Buffer

	Buffer::Buffer(Buffer&& rhs) noexcept
	{
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);

		rhs.m_bufferCpu->Release();
		rhs.m_bufferGpu->Release();
		rhs.m_bufferUploader->Release();

	}

	Buffer& Buffer::operator=(Buffer&& rhs) noexcept
	{
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);

		rhs.m_bufferCpu->Release();
		rhs.m_bufferGpu->Release();
		rhs.m_bufferUploader->Release();

		return *this;
	}

	Buffer::~Buffer()
	{
		DisposeUploader();
	}

	void Buffer::Create(const D3DCommandContext& commandContext, const void* initData, std::uint64_t byteSize)
	{
		auto deviceManager = commandContext.GetDeviceManager();
		auto d3dDevice = deviceManager->GetD3DDevice();
		auto cmdList = commandContext.GetCommandList();

		// Create the Blob
		DXThrowIfFailed(
			D3DCreateBlob(byteSize, m_bufferCpu.ReleaseAndGetAddressOf()));

		// Copy data into the blob
		CopyMemory(m_bufferCpu->GetBufferPointer(), initData, byteSize);

		// Create the DefaultBuffer for GPU
		m_bufferGpu = D3DUtility::CreateDefaultBuffer(d3dDevice, cmdList.Get(),
			initData, byteSize, m_bufferUploader);
	}

	void Buffer::DisposeUploader()
	{
		m_bufferUploader.Reset();
	}



	// IndexBuffer

	IndexBuffer::IndexBuffer(const DXGI_FORMAT& dataFormat) : m_dataFormat(dataFormat)
	{
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& rhs) noexcept
	{
		this->m_dataFormat = rhs.m_dataFormat;
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);
			}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& rhs) noexcept
	{
		this->m_dataFormat = rhs.m_dataFormat;
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);

		return *this;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBuffer::GetView() const
	{
		return D3D12_INDEX_BUFFER_VIEW{ m_bufferGpu->GetGPUVirtualAddress(),
			static_cast<UINT>(m_bufferCpu->GetBufferSize()),
			m_dataFormat };
	}



	// VertexBuffer

	VertexBuffer::VertexBuffer(std::uint16_t strideInBytes) : m_byteStride(strideInBytes)
	{
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs) noexcept
	{
		this->m_byteStride = rhs.m_byteStride;
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);

	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs) noexcept
	{
		this->m_byteStride = rhs.m_byteStride;
		this->m_bufferCpu = std::move(rhs.m_bufferCpu);
		this->m_bufferGpu = std::move(rhs.m_bufferGpu);
		this->m_bufferUploader = std::move(rhs.m_bufferUploader);

		return *this;
	}

	D3D12_VERTEX_BUFFER_VIEW VertexBuffer::GetView() const
	{
		return D3D12_VERTEX_BUFFER_VIEW{m_bufferGpu->GetGPUVirtualAddress(),
			static_cast<UINT>(m_bufferCpu->GetBufferSize()),
			m_byteStride};
	
	}
}
