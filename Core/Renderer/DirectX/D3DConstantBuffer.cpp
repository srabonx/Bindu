#include "D3DConstantBuffer.h"

namespace BINDU
{
	D3DConstantBuffer::D3DConstantBuffer(std::uint64_t elementCount) : m_uploadBuffer(true), m_memoryAllocator(elementCount), m_bufferSize(elementCount)
	{
	}

	std::uint64_t D3DConstantBuffer::Allocate()
	{
		return m_memoryAllocator.Allocate(1) - 1;
	}

	void D3DConstantBuffer::Free(std::uint64_t elementIndex)
	{
		m_memoryAllocator.Free(elementIndex + 1, 1);
	}

	D3D12_GPU_VIRTUAL_ADDRESS D3DConstantBuffer::GetGpuAddressAt(std::uint64_t elementIndex) const
	{
		return m_uploadBuffer.GetGPUVirtualAddressAt(elementIndex);
	}
}
