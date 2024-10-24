#include "DynamicVoidBuffer.h"

#include <cstring>
#include <new>

namespace BINDU
{
	DynamicVoidBuffer::DynamicVoidBuffer(size_t initialSize) : m_currSize(0), m_totalSize(initialSize)
	{
		m_buffer = malloc(initialSize);
		if (!m_buffer)
			throw std::bad_alloc();
	}

	DynamicVoidBuffer::~DynamicVoidBuffer()
	{
		Free();
	}

	DynamicVoidBuffer::DynamicVoidBuffer(DynamicVoidBuffer&& rhs) noexcept : m_buffer(rhs.m_buffer),
										m_currSize(rhs.m_currSize), m_totalSize(rhs.m_totalSize)
	{
		// Steal the buffer 
		rhs.m_buffer = nullptr;
		rhs.m_currSize = 0;
		rhs.m_totalSize = 0;
	}

	DynamicVoidBuffer& DynamicVoidBuffer::operator=(DynamicVoidBuffer&& rhs) noexcept
	{
		if (this != &rhs)
		{
			// Free the existing buffer
			if (m_buffer) {
				free(m_buffer);
			}

			// Steal the buffer and internal state from 'rhs'
			m_buffer = rhs.m_buffer;
			m_currSize = rhs.m_currSize;
			m_totalSize = rhs.m_totalSize;

			// Reset 'rhs' to a safe state
			rhs.m_buffer = nullptr;
			rhs.m_currSize = 0;
			rhs.m_totalSize = 0;
		}
		return *this;
	}


	void DynamicVoidBuffer::Append(const void* data, size_t dataSizeInBytes)
	{
		// check if we need to grow the buffer
		if (m_currSize + dataSizeInBytes > m_totalSize)
			Grow(m_currSize + dataSizeInBytes);

		// append the new data
		std::memcpy(static_cast<char*>(m_buffer) + m_currSize, data, dataSizeInBytes);
		m_currSize += dataSizeInBytes;
	}

	void* DynamicVoidBuffer::GetBufferData() const
	{
		return m_buffer;
	}

	size_t DynamicVoidBuffer::GetBufferSize() const
	{
		return m_currSize;
	}

	void DynamicVoidBuffer::Reset()
	{
		m_currSize = 0;
	}

	void DynamicVoidBuffer::Free()
	{
		if (m_buffer)
		{
			free(m_buffer);
			m_buffer = nullptr;
		}
	}

	void DynamicVoidBuffer::Grow(size_t newSize)
	{
		m_totalSize = newSize;

		// Reallocate the buffer
		void* newBuffer = realloc(m_buffer, newSize);

		if (!newBuffer)
			throw std::bad_alloc();

		// make the buffer point to the new allocated buffer
		m_buffer = newBuffer;
	}
}
