#ifndef DYNAMICBITSET_H
#define DYNAMICBITSET_H

#include <cstdint>
#include <vector>

namespace BINDU
{
	class DynamicBitset
	{
	public:
		DynamicBitset() = default;
		~DynamicBitset() = default;

		// Ensure bitset has enough size to accommodate the given index
		void	EnsureSize(size_t index);

		// Set the bit at the given index to 1
		void	Set(size_t index);

		// Clear the bit at the given index to 0
		void	Clear(size_t index);

		// Flip the bit at the given index
		void	Flip(size_t index);

		// Test to see whether the bit at the given index is set to 1
		bool	Test(size_t index) const;


	private:
		static constexpr size_t BITS_PER_BLOCK = 64;
		std::vector<std::uint64_t> m_bitBlocks;
	};
}

#endif