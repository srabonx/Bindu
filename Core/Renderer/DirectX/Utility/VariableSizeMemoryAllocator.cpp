#include "VariableSizeMemoryAllocator.h"

namespace BINDU
{
	VariableSizeMemoryAllocator::VariableSizeMemoryAllocator(size_t size) : m_freeSize(size)
	{
		
	}

	std::uint32_t VariableSizeMemoryAllocator::Allocate(size_t size)
	{

		// If not enough free memory
		if (m_freeSize < size)
			return InvalidOffset;

		// Find the first block with the least amount of free memory to accomodate a memory block of size
		auto smallestBlock = m_sizeMap.lower_bound(size);

		// If there is no free memory that can accomodate a memory block of size
		if (smallestBlock == m_sizeMap.end())
			return InvalidOffset;

		auto smallestBlockIt = smallestBlock->second;

		auto offset = smallestBlockIt->first;

		auto newOffset = offset + static_cast<std::uint32_t>(size);

		auto newSize = smallestBlockIt->second.Size - size;

		// Remove the previous entries
		m_offsetMap.erase(smallestBlockIt);

		m_sizeMap.erase(smallestBlock);

		if (newSize > 0)
			AddNewBlock(newOffset, newSize);

		m_freeSize -= size;

		return offset;
	}

	void VariableSizeMemoryAllocator::Free(std::uint32_t offset, size_t size)
	{
		// Find the first element whose offset is greater than the specified offset
		auto nextBlock = m_offsetMap.upper_bound(offset);
		auto prevBlock = nextBlock;

		if (prevBlock != m_offsetMap.begin())
			--prevBlock;
		else
			prevBlock = m_offsetMap.end();

		std::uint32_t newOffset{ 0 };
		size_t		  newSize{ 0 };

		if(prevBlock != m_offsetMap.end() && offset == prevBlock->first + prevBlock->second.Size)
		{
			newOffset = prevBlock->first;
			newSize = size + prevBlock->second.Size;

			if(nextBlock != m_offsetMap.end() && offset + size == nextBlock->first)
			{
				newSize += nextBlock->second.Size;

				// delete the size entry of two blocks
				m_sizeMap.erase(prevBlock->second.SizeMapIt);
				m_sizeMap.erase(nextBlock->second.SizeMapIt);

				// Go to the next free block
				++nextBlock;

				// delete the range of two blocks
				m_offsetMap.erase(prevBlock, nextBlock);
			}
			else
			{
				m_sizeMap.erase(prevBlock->second.SizeMapIt);
				m_offsetMap.erase(prevBlock);
			}

		}
		else if(nextBlock != m_offsetMap.end() && offset + size == nextBlock->first)
		{
			newSize = size + nextBlock->second.Size;
			newOffset = offset;

			m_offsetMap.erase(nextBlock);
			m_sizeMap.erase(nextBlock->second.SizeMapIt);
		}
		else
		{
			newSize = size;
			newOffset = offset;
		}

		AddNewBlock(newOffset, newSize);

		m_freeSize += size;
	}

	void VariableSizeMemoryAllocator::AddNewBlock(std::uint32_t offset, size_t size)
	{
		auto newBlockIt = m_offsetMap.emplace(offset, size);

		auto orderIt = m_sizeMap.emplace(size, newBlockIt.first);

		newBlockIt.first->second.SizeMapIt = orderIt;
	}
}
