
#include "VariableSizeGpuMemoryAllocator.h"

namespace BINDU
{
	VariableSizeGpuMemoryAllocator::VariableSizeGpuMemoryAllocator(size_t maxSize) : VariableSizeMemoryAllocator(maxSize)
	{

	}

	VariableSizeGpuMemoryAllocator::~VariableSizeGpuMemoryAllocator()
	{
	}

	void VariableSizeGpuMemoryAllocator::Free(std::uint32_t offset, size_t size, std::uint64_t frameNumber)
	{
		// Push it into the queue for later remove
		m_staleAllocations.emplace_back(offset, size, frameNumber);
	}

	void VariableSizeGpuMemoryAllocator::ReleaseCompletedFrames(std::uint64_t numOfCompletedFrames)
	{
		while (!m_staleAllocations.empty() && m_staleAllocations.front().FrameNumber < numOfCompletedFrames)
		{
			auto& oldestAllocation = m_staleAllocations.front();
			VariableSizeMemoryAllocator::Free(oldestAllocation.Offset, oldestAllocation.Size);
			m_staleAllocations.pop_front();
		}
	}
}