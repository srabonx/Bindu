#ifndef GPUDESCRIPTORHEAP_H
#define GPUDESCRIPTORHEAP_H


#include <set>
#include <vector>

#include "DescriptorHeapManager.h"


namespace BINDU
{

	class GpuDescriptorHeap : public IDescriptorHeapAllocator
	{
	public:
		GpuDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType);

		~GpuDescriptorHeap();

		DescriptorHeapAllocation	Allocate(size_t count) override;

		DescriptorHeapAllocation	AllocateDynamic(size_t count);

		void						Free(DescriptorHeapAllocation&& allocation) override;

		void						ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames) override;

		DescriptorHeap*				GetDescriptorHeap() const;

	private:
		// Pointer to the Direct3D device
		ID3D12Device*							m_d3dDevice{ nullptr };

		// Description of the descriptor heaps
		D3D12_DESCRIPTOR_HEAP_DESC				m_descriptorHeapDesc;

		// Main descriptor heap that will be managed by two descriptor heap manager
		// One descriptorHeapManager will handle the static or mutable allocations and the other will handle dynamic allocations
		std::shared_ptr<DescriptorHeap>			m_descriptorHeap{ nullptr };

		// Static and Mutable allocations manager
		std::unique_ptr<DescriptorHeapManager>	m_staticHeapManager{ nullptr };

		// Static heap managers offset into the descriptor heap
		std::uint32_t							m_staticHeapManagerOffset{ 0 };

		// Count of descriptors managed by Static heap managers in the descriptor heap
		std::uint32_t							m_staticHeapManagerDescriptorCount{ 300 };

		// Dynamic allocations manager
		std::unique_ptr<DescriptorHeapManager>	m_dynamicHeapManager{ nullptr };

		// Dynamic heap managers offset into the descriptor heap
		std::uint32_t							m_dynamicHeapManagerOffset{ 301 };

		// Count of descriptors managed by Dynamic heap managers in the descriptor heap
		std::uint32_t							m_dynamicHeapManagerDescriptorCount{ 700 };

		// Heap manager id
		std::uint16_t							m_staticHeapManagerId{ 0 };

		std::uint16_t							m_dynamicHeapManagerId{ 1 };

		// Current count of total descriptors
		size_t									m_currentSize{ 0 };

	};




	// Manager class to handle sub allocations from the dynamicHeap of GpuDescriptorHeap
	class DynamicSubAllocationManager : public IDescriptorHeapAllocator
	{
	public:
		explicit DynamicSubAllocationManager(GpuDescriptorHeap* parentGpuDescHeap);

		~DynamicSubAllocationManager();

		DescriptorHeapAllocation				Allocate(size_t count) override;

		void									Free(DescriptorHeapAllocation&& allocation) override;

		void									ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames) override;

		// Discard all the allocations
		void									DiscardAllocations(std::uint64_t frameNumber);

	private:

		// Parent Gpu Descriptor Heap
		GpuDescriptorHeap*						m_parentGpuHeap{ nullptr };

		// sub allocations
		std::vector<DescriptorHeapAllocation>	m_subAllocations;

		// offset into the current sub allocation
		std::uint32_t							m_currentSubAllocationOffset{ 0 };

		// Size of the default chunk
		size_t									m_dynamicChunkSize{ 1 };
	};


}

#endif