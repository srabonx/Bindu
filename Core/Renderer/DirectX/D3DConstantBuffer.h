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
		D3DConstantBuffer(std::uint64_t elementCount);

		~D3DConstantBuffer() = default;

		template<typename T>
		void Initialize(D3DDeviceManager* deviceManager)
		{
			m_uploadBuffer.Initialize<T>(deviceManager->GetD3DDevice(), m_bufferSize);
		}

		// Allocates space for an element in the buffer and returns the index of the element in the buffer
		std::uint64_t		Allocate();

		// Frees the allocated space for the element at elementIndex in the buffer
		void				Free(std::uint64_t elementIndex);

		template<typename T>
		void UpdateBuffer(std::uint64_t elementIndex, const T& data)
		{
			m_uploadBuffer.CopyData(elementIndex, data);
		}

		D3D12_GPU_VIRTUAL_ADDRESS	GetGpuAddressAt(std::uint64_t elementIndex) const;

	private:

		UploadBuffer							m_uploadBuffer;

		VariableSizeMemoryAllocator				m_memoryAllocator;

		std::uint64_t							m_bufferSize{ 0 };

	};
}

#endif