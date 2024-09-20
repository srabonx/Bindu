#ifndef IDESCRIPTORHEAPALLOCATOR_H
#define IDESCRIPTORHEAPALLOCATOR_H

#include <cstdint>

namespace BINDU
{
	class DescriptorHeapAllocation;

	class IDescriptorHeapAllocator
	{
	public:
		virtual ~IDescriptorHeapAllocator() {}

		// Allocate the specified count of descriptors from DescriptorHeap
		virtual DescriptorHeapAllocation Allocate(size_t count) = 0;

		// Free the specified allocation
		virtual void					 Free(DescriptorHeapAllocation&& allocation) = 0;

		// Releases all stale allocations 
		virtual void					 ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames) = 0;

	};
}

#endif
