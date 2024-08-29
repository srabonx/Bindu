#ifndef VARIABLESIZEGPUMEMORYALLOCATOR_H
#define VARIABLESIZEGPUMEMORYALLOCATOR_H

#include "VariableSizeMemoryAllocator.h"
#include <deque>

namespace BINDU
{
	class VariableSizeGpuMemoryAllocator : public VariableSizeMemoryAllocator
	{

	public:

		VariableSizeGpuMemoryAllocator(size_t maxSize);

		~VariableSizeGpuMemoryAllocator();

		void		Free(std::uint32_t offset, size_t size, std::uint64_t frameNumber);

		void		ReleaseCompletedFrames(std::uint64_t numOfCompletedFrames);


	private:

		struct FreedAllocationInfo
		{
			std::uint32_t		Offset{ 0 };
			size_t				Size{ 0 };
			std::uint64_t		FrameNumber{ 0 };

			FreedAllocationInfo(std::uint32_t offset, size_t size, std::uint64_t frameNumber) : Offset(offset), Size(size), FrameNumber(frameNumber)
			{}
		};

		std::deque<FreedAllocationInfo>		m_staleAllocations;
	};
}

#endif