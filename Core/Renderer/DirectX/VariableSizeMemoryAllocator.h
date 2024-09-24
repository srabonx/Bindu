#ifndef VARIABLESIZEMEMORYALLOCATOR_H
#define VARIABLESIZEMEMORYALLOCATOR_H

#include <map>
#include <cstdint>


namespace BINDU
{
	constexpr std::uint64_t InvalidOffset = UINT64_MAX;

	class VariableSizeMemoryAllocator
	{
	public:

		~VariableSizeMemoryAllocator() = default;

		// initialSize = The memory size this allocator will handle initially
		explicit VariableSizeMemoryAllocator(size_t initialSize);

		// move constructor
		VariableSizeMemoryAllocator(VariableSizeMemoryAllocator&& rhs) noexcept;

		// move assignment
		VariableSizeMemoryAllocator& operator = (VariableSizeMemoryAllocator&& rhs) noexcept;

		// No copy constructor
		VariableSizeMemoryAllocator(VariableSizeMemoryAllocator& rhs) = delete;

		// No copy assignment
		VariableSizeMemoryAllocator& operator = (VariableSizeMemoryAllocator& rhs) = delete;


		// Returns the offset where size amount of memory is available
		std::uint64_t		Allocate(size_t size);

		// Frees the size amount of memory at the given offset
		void				Free(std::uint64_t offset, size_t size);

	private:

		// Allocates a new memory block at the given offset
		void				AddNewBlock(std::uint64_t offset, size_t size);


	private:

		// Holds information about the free memory block
		struct FreeBlockInfo;

		using offset = std::uint64_t;

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

		// Orders the free blocks by size
		SizeMap						m_sizeMap;

		// Free memory size
		size_t						m_freeSize{ 128 };
	};

}
#endif