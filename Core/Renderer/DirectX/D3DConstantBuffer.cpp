#include "D3DConstantBuffer.h"

namespace BINDU
{
	D3DConstantBuffer::D3DConstantBuffer(std::uint64_t elementCount, std::uint16_t bufferId) : m_uploadBuffer(true),
																								m_memoryAllocator(elementCount),
																								m_bufferSize(elementCount),
																								m_freeSize(elementCount),
																								m_bufferId(bufferId)
	{
	}

	D3DConstantBuffer::CBAllocation D3DConstantBuffer::Allocate(std::uint64_t size)
	{
		CBAllocation allocation;
		allocation.CBIndex = m_memoryAllocator.Allocate(size);
		allocation.Size = size;
		allocation.BufferId = m_bufferId;

		m_freeSize -= size;

		return allocation;
	}

	void D3DConstantBuffer::Free(std::uint64_t elementIndex, std::uint64_t size)
	{
		m_memoryAllocator.Free(elementIndex, size);

		m_freeSize += size;
	}

	
	D3D12_GPU_VIRTUAL_ADDRESS D3DConstantBuffer::GetGpuAddressAt(std::uint64_t elementIndex) const
	{
		return m_uploadBuffer.GetGPUVirtualAddressAt(elementIndex);
	}

	std::uint16_t D3DConstantBuffer::GetBufferId() const
	{
		return m_bufferId;
	}

	std::uint64_t D3DConstantBuffer::GetFreeSize() const
	{
		return m_freeSize;
	}
}
