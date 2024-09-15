#include "Buffers.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DRenderDevice.h"
#include "../Renderer/DirectX/Utility/D3DUtillity.h"

namespace BINDU
{

	// Buffer

	Buffer::~Buffer()
	{
		DisposeUploader();
	}

	void Buffer::Create(const D3DCommandContext& commandContext, const void* initData, std::uint64_t byteSize)
	{
		auto device = commandContext.GetParentDevice()->GetD3DDevice();
		auto cmdList = commandContext.GetCommandList();

		// Create the Blob
		DXThrowIfFailed(
			D3DCreateBlob(byteSize, m_bufferCpu.ReleaseAndGetAddressOf()));

		// Copy data into the blob
		CopyMemory(m_bufferCpu->GetBufferPointer(), initData, byteSize);

		// Create the DefaultBuffer for GPU
		m_bufferGpu = D3DUtility::CreateDefaultBuffer(device, cmdList.Get(),
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

	D3D12_VERTEX_BUFFER_VIEW VertexBuffer::GetView() const
	{
		return D3D12_VERTEX_BUFFER_VIEW{m_bufferGpu->GetGPUVirtualAddress(),
			static_cast<UINT>(m_bufferCpu->GetBufferSize()),
			m_byteStride};
	
	}
}
