#ifndef CPUDESCRIPTORHEAP_H
#define CPUDESCRIPTORHEAP_H

#include <vector>
#include "DescriptorHeapManager.h"
#include <set>

namespace BINDU
{
	// This class manages a pool of DescriptorHeapManager
	class CpuDescriptorHeap : public IDescriptorHeapAllocator
	{
	public:
		CpuDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
		
		~CpuDescriptorHeap();

		void						Initialize() override;

		// Allocates the specified amount of descriptors in the heap and returns the allocation
		DescriptorHeapAllocation	Allocate(size_t count) override;

		// Free's the allocation from the heap and put's it into a release queue
		void						Free(DescriptorHeapAllocation&& allocation) override;

		// Releases all the stale allocations from the queue
		void						ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames) override;

	private:

		// Pointer to the Direct3D device
		ID3D12Device*							m_d3dDevice{ nullptr };

		// Description of the descriptor heaps
		D3D12_DESCRIPTOR_HEAP_DESC				m_descriptorHeapDesc;

		// Pool of DescriptorHeapManager
		std::vector<DescriptorHeapManager>		m_descriptorHeapPool;

		// Indices of available DescriptorHeaps
		std::set<size_t>						m_availableDescriptorHeaps;

		// Current size of the total heaps
		size_t									m_currentSize{ 0 };

		// The maximum size of this CpuDescriptorHeap
		size_t									m_maxHeapSize{ 0 };
	};
}

#endif