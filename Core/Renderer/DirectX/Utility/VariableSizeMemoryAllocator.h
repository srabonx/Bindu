#ifndef VARIABLESIZEMEMORYALLOCATOR_H
#define VARIABLESIZEMEMORYALLOCATOR_H

#include <map>
#include <cstdint>


namespace BINDU
{
	constexpr std::uint32_t InvalidOffset = UINT32_MAX;

	class VariableSizeMemoryAllocator
	{
	public:

		~VariableSizeMemoryAllocator() = default;

		// initialSize = The memory size this allocator will handle initially
		explicit VariableSizeMemoryAllocator(size_t initialSize);

		// No copy constructor
		VariableSizeMemoryAllocator(const VariableSizeMemoryAllocator& rhs) = delete;

		// No copy assignment
		VariableSizeMemoryAllocator& operator = (const VariableSizeMemoryAllocator& rhs) = delete;

		// No move constructor
		VariableSizeMemoryAllocator(const VariableSizeMemoryAllocator&& rhs) = delete;

		// No move assignment
		VariableSizeMemoryAllocator& operator = (const VariableSizeMemoryAllocator&& rhs) = delete;

		// Returns the offset where size amount of memory is available
		std::uint32_t		Allocate(size_t size);

		// Frees the size amount of memory at the given offset
		void				Free(std::uint32_t offset, size_t size);

	private:

		// Allocates a new memory block at the given offset
		void				AddNewBlock(std::uint32_t offset, size_t size);


	private:

		// Holds information about the free memory block
		struct FreeBlockInfo;

		using offset = std::uint32_t;

		using OffsetMap = std::map < offset, FreeBlockInfo>;

		// Size map holds the size of free blocks as key and iterators to the offsets that has the key size amount of free blocks
		using SizeMap = std::multimap<size_t, OffsetMap::iterator>;


		struct FreeBlockInfo
		{
			size_t					Size{ 0 };

			// Iterator referencing this blocks position in the SizeMap
			SizeMap::iterator		SizeMapIt;

			FreeBlockInfo(size_t size) : Size(size) {}
		};


		// Orders the free blocks by offset
		OffsetMap					m_offsetMap;

		// Offset map iterator
		OffsetMap::iterator			m_offsetMapIt;

		// Orders the free blocks by size
		SizeMap						m_sizeMap;

		// Free memory size
		size_t						m_freeSize{ 128 };
	};

}
#endif