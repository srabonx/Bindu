#include "DirectXConstantBuffer.h"

#include "D3DUtillity.h"

namespace BINDU
{
	DirectXConstantBuffer::DirectXConstantBuffer(std::uint32_t elementCount) : m_elementCount(elementCount)
	{
		m_elementByteSize = D3DUtility::CalculateConstantBufferByteSize(256);

	}

	DirectXConstantBuffer::~DirectXConstantBuffer()
	{
		if (m_resource)
			m_resource->Unmap(0, nullptr);
	}

	DirectXConstantBuffer::DirectXConstantBuffer(DirectXConstantBuffer&& other)	noexcept
	{
		std::swap(this->m_mappedData, other.m_mappedData);
		std::swap(this->m_resource, other.m_resource);
		std::swap(this->m_elementCount, other.m_elementCount);
		std::swap(this->m_elementByteSize, other.m_elementByteSize);

		other.m_resource.Reset();
	}

	DirectXConstantBuffer& DirectXConstantBuffer::operator=(DirectXConstantBuffer&& other) noexcept
	{
		std::swap(this->m_mappedData, other.m_mappedData);
		std::swap(this->m_resource, other.m_resource);
		std::swap(this->m_elementCount, other.m_elementCount);
		std::swap(this->m_elementByteSize, other.m_elementByteSize);

		other.m_resource.Reset();

		return *this;
	}

	void DirectXConstantBuffer::SetData(const void* data, std::uint32_t size, std::uint32_t offset)
	{
		memcpy(&m_mappedData[static_cast<size_t>(offset * m_elementByteSize)], data, size);
	}

	D3D12_GPU_VIRTUAL_ADDRESS DirectXConstantBuffer::GetGPUVirtualAddressAt(std::uint32_t index) const
	{
		return m_resource->GetGPUVirtualAddress() + (m_elementByteSize * index);
	}
}
