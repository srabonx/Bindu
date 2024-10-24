#ifndef DYNAMICVOIDBUFFER_H
#define DYNAMICVOIDBUFFER_H

#include <cstdlib>

namespace BINDU
{
	class DynamicVoidBuffer
	{
	public:
		explicit DynamicVoidBuffer(size_t initialSize);

		~DynamicVoidBuffer();

		// Move Constructor
		DynamicVoidBuffer(DynamicVoidBuffer&& rhs) noexcept;

		// Move Assignment Operator
		DynamicVoidBuffer& operator=(DynamicVoidBuffer&& rhs) noexcept;

		// Appends the data into the buffer
		void	Append(const void* data, size_t dataSizeInBytes);

		// returns pointer to the buffer
		void*	GetBufferData() const;

		// Returns buffer size in byte
		size_t	GetBufferSize() const;

		// Resets the buffer for refilling
		void	Reset();

		// Free the buffer memory
		void	Free();

	private:
		// Grows the buffer to the requested size
		void	Grow(size_t newSize);

	private:

		// Internal void* buffer
		void*	m_buffer{ nullptr };

		// Current size of the buffer
		size_t	m_currSize{ 0 };

		// Total size of the buffer
		size_t	m_totalSize{ 0 };
	};
}

#endif