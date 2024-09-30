#ifndef IDESCRIPTORHEAPALLOCATOR_H
#define IDESCRIPTORHEAPALLOCATOR_H

#include <cstdint>
#include <memory>

namespace BINDU
{
	class DescriptorHeapAllocation;

	class IDescriptorHeapAllocator : public std::enable_shared_from_this<IDescriptorHeapAllocator>
	{
	public:
		virtual ~IDescriptorHeapAllocator() {}

		virtual void					 Initialize() = 0;

		// Allocate the specified count of descriptors from DescriptorHeap
		virtual DescriptorHeapAllocation Allocate(size_t count) = 0;

		// Free the specified allocation
		virtual void					 Free(DescriptorHeapAllocation&& allocation) = 0;

		// Releases all stale allocations 
		virtual void					 ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames) = 0;

	};
}

#endif
