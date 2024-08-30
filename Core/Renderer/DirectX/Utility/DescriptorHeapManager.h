#ifndef DESCRIPTORHEAPMANAGER_H
#define DESCRIPTORHEAPMANAGER_H


#include "VariableSizeGpuMemoryAllocator.h"
#include "DescriptorHeap.h"


namespace BINDU
{
	class DescriptorHeapAllocation
	{
	public:

		DescriptorHeapAllocation() = default;

		DescriptorHeapAllocation(const std::shared_ptr<DescriptorHeap>& pDescriptorHeap,D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
			size_t numOfHandles, std::uint32_t managerId);

		~DescriptorHeapAllocation();

		D3D12_CPU_DESCRIPTOR_HANDLE	GetCpuHandle(std::uint32_t offset = 0) const;

		D3D12_GPU_DESCRIPTOR_HANDLE	GetGpuHandle(std::uint32_t offset = 0) const;

		// Returns the number of handles this allocation has
		size_t						GetNumOfHandles() const;

		// Returns if this allocation is null or not
		bool						IsNull() const;

		// Returns if this allocation is shader visible or not
		bool						IsShaderVisible() const;

		// Returns this allocations manager is
		std::uint32_t				GetManagerId() const;

		// Returns the descriptor increment size of this allocation
		std::uint32_t				GetDescriptorIncrementSize() const;

	private:
		std::weak_ptr<DescriptorHeap>		m_descriptorHeap;

		// first cpu handle in the allocation
		D3D12_CPU_DESCRIPTOR_HANDLE			m_firstCpuHandle{ 0 };

		// First Gpu handle in te allocation
		D3D12_GPU_DESCRIPTOR_HANDLE			m_firstGpuHandle{ 0 };

		// Number of handles in this allocation
		size_t								m_numOfHandles{ 0 };

		// Manager id of this allocation
		std::uint32_t						m_managerId{ 0 };

		// Descriptor increment size
		std::uint32_t						m_descriptorIncrementSize{ 0 };
	};



	class DescriptorHeapManager
	{
	public:
		DescriptorHeapManager(ID3D12Device* pDevice,const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, std::uint32_t managerId);

		// To manage a sub-range of descriptors from a DescriptorHeap
		DescriptorHeapManager(ID3D12Device* pDevice, DescriptorHeap* pDescriptorHeap, std::uint32_t managerId,
			std::uint32_t offset, size_t numOfDescriptors);

		~DescriptorHeapManager();

		// Allocate count amount of descriptors
		DescriptorHeapAllocation	Allocate(size_t count);

		// Releases the descriptor heap allocation
		// Note: This doesnt releases the allocation immediately. The allocation gets added to the Release queue and will be
		// released once ReleaseStaleAllocations gets called
		void						Free(DescriptorHeapAllocation& allocation);

		// Releases all stale allocations 
		void						ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames);

		// Returns the available descriptors
		size_t						GetAvailableDescriptorCount() const;


	private:

		// Pointer to the device that created this manager
		ID3D12Device*						m_d3dDevice{ nullptr };

		// Pointer to the descriptor this manager will manage
		std::shared_ptr<DescriptorHeap>		m_descriptorHeap{ nullptr };

		// Memory allocator to help manage the allocation and deallocation
		VariableSizeGpuMemoryAllocator		m_freeMemAllocator;

		// First cpu Descriptor Handle in the available descriptor range
		D3D12_CPU_DESCRIPTOR_HANDLE			m_firstCpuHandle{ 0 };

		// First gpu Descriptor Handle in the available descriptor range
		D3D12_GPU_DESCRIPTOR_HANDLE			m_firstGpuHandle{ 0 };

		// Free descriptors
		size_t								m_freeDescriptors{ 0 };

		// Id of this manager
		std::uint32_t						m_managerId{ 0 };
	};
}

#endif
