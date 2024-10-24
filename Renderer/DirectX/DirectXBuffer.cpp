#include "DirectXBuffer.h"

#include "D3DUtillity.h"

namespace BINDU
{
	/////	Vertex Buffer /////

	DirectXVertexBuffer::DirectXVertexBuffer(const ComPtr<ID3D12GraphicsCommandList>& commandList) : m_commandList(commandList)
	{
	}

	void DirectXVertexBuffer::Bind()
	{
		auto vbv = GetVertexBufferView();

		m_commandList->IASetVertexBuffers(0, 1, &vbv);
	}

	D3D12_VERTEX_BUFFER_VIEW DirectXVertexBuffer::GetVertexBufferView() const
	{
		return { m_bufferGpu->GetGPUVirtualAddress(), m_sizeInBytes, m_byteStride };
	}

	void DirectXVertexBuffer::DisposeUploader()
	{
		m_bufferUploader.Reset();
	}


	///// Index Buffer /////

	DirectXIndexBuffer::DirectXIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& commandList) : m_commandList(commandList)
	{
	}

	void DirectXIndexBuffer::Bind()
	{
		auto ibv = GetIndexBufferView();
		m_commandList->IASetIndexBuffer(&ibv);
	}

	std::uint32_t DirectXIndexBuffer::GetCount() const
	{
		return m_count;
	}

	D3D12_INDEX_BUFFER_VIEW DirectXIndexBuffer::GetIndexBufferView() const
	{
		return { m_bufferGpu->GetGPUVirtualAddress(), m_sizeInBytes, m_dxgiFormat };
	}

	void DirectXIndexBuffer::DisposeUploader()
	{
		m_bufferUploader.Reset();
	}
}
