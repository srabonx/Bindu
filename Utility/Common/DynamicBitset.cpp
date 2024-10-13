#include "DynamicBitset.h"

namespace BINDU
{
	void DynamicBitset::EnsureSize(size_t index)
	{
		auto bitBlock = (index / BITS_PER_BLOCK) + 1;

		if(bitBlock > m_bitBlocks.size())
		{
			// We need to accommodate a new block
			m_bitBlocks.resize(bitBlock, 0);
		}
	}

	void DynamicBitset::Set(size_t index)
	{
		// Ensure this bit can be set
		EnsureSize(index);

		m_bitBlocks[index / BITS_PER_BLOCK] |= (1ULL << (index % BITS_PER_BLOCK));
	}

	void DynamicBitset::Clear(size_t index)
	{
		// Ensure this bit can be cleared
		EnsureSize(index);

		m_bitBlocks[index / BITS_PER_BLOCK] &= ~(1ULL << (index % BITS_PER_BLOCK));
	}

	void DynamicBitset::Flip(size_t index)
	{
		// Ensure this bit can be flipped
		EnsureSize(index);

		m_bitBlocks[index / BITS_PER_BLOCK] ^= (1ULL << (index % BITS_PER_BLOCK));
	}

	bool DynamicBitset::Test(size_t index) const
	{
		auto blockIndex = index / BITS_PER_BLOCK;

		if (blockIndex > m_bitBlocks.size())
			return false;	// Treat out of bounds as not set.

		return m_bitBlocks[index / BITS_PER_BLOCK] & (1ULL << (index % BITS_PER_BLOCK));
	}
}