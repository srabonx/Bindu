#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include <memory>

#include "D3DDeviceManager.h"
#include "UploadBuffer.h"
#include "VariableSizeMemoryAllocator.h"

namespace BINDU
{


	class D3DConstantBuffer
	{
	public:
		// Constant Buffer allocation
		struct CBAllocation
		{
			std::uint64_t		CBIndex{ 0 };

			std::uint64_t		Size{ 0 };

			std::uint64_t		BufferId{ 0 };
		};

	public:
		D3DConstantBuffer(std::uint64_t elementCount, std::uint16_t bufferId = 0);

		~D3DConstantBuffer() = default;

		void Initialize(D3DDeviceManager* deviceManager, std::uint32_t cbDataByteSize)
		{
			m_uploadBuffer.Initialize(deviceManager->GetD3DDevice(), m_bufferSize, cbDataByteSize);
		}

		// Allocates space for an element in the buffer and returns the allocation of the element in the buffer
		CBAllocation		Allocate(std::uint64_t size = 1);

		// Frees the allocated space for the element at elementIndex in the buffer
		void				Free(std::uint64_t elementIndex, std::uint64_t size = 1);

		template<typename T>
		void UpdateBuffer(std::uint64_t elementIndex, const T& data)
		{
			m_uploadBuffer.CopyData(elementIndex, data);
		}

		D3D12_GPU_VIRTUAL_ADDRESS	GetGpuAddressAt(std::uint64_t elementIndex) const;

		std::uint16_t		GetBufferId() const;

		std::uint64_t		GetFreeSize() const;

	private:

		UploadBuffer							m_uploadBuffer;

		VariableSizeMemoryAllocator				m_memoryAllocator;

		std::uint64_t							m_bufferSize{ 0 };

		std::uint64_t							m_freeSize{ 0 };

		std::uint16_t							m_bufferId{ 0 };
	};
}

#endif