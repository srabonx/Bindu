
#define NOMINMAX

#include "CpuDescriptorHeap.h"

namespace BINDU
{
	CpuDescriptorHeap::CpuDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType) : m_d3dDevice(pDevice)
	{
		m_descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_descriptorHeapDesc.NodeMask = 0;

		// 128 Descriptors as default
		m_descriptorHeapDesc.NumDescriptors = 128;
		m_descriptorHeapDesc.Type = heapType;
	}

	CpuDescriptorHeap::~CpuDescriptorHeap()
	{
	}

	void CpuDescriptorHeap::Initialize()
	{
	}

	DescriptorHeapAllocation CpuDescriptorHeap::Allocate(size_t count)
	{
		DescriptorHeapAllocation allocation;

		// Go through all the available DescriptorHeapManagers
		for(auto itr = m_availableDescriptorHeaps.begin(); itr!=m_availableDescriptorHeaps.end();++itr)
		{
			// Try to allocate using the current manager
			allocation = m_descriptorHeapPool[*itr].Allocate(count);

			// If current manager is full then remove it from the available heap list
			if (m_descriptorHeapPool[*itr].GetAvailableDescriptorCount() == 0)
				m_availableDescriptorHeaps.erase(*itr);

			// The CpuHandle ptr will be > 0 if an allocation was successful, so we break out if an allocation is successful
			if (allocation.GetCpuHandle().ptr != 0)
				break;
		}

		// If no allocation was successful then we create a new manager and allocate there.
		if(allocation.GetCpuHandle().ptr == 0)
		{
			// The heap has to be large enough to accomodate the requested size
			m_descriptorHeapDesc.NumDescriptors = std::max(m_descriptorHeapDesc.NumDescriptors, static_cast<UINT>(count));

			// Create a new DescriptorHeapManager, use descriptorHeapPools size as manager id.
			m_descriptorHeapPool.emplace_back(shared_from_this(), m_d3dDevice, m_descriptorHeapDesc,
				static_cast<std::uint32_t>(m_descriptorHeapPool.size()));

			// Put it into availableDescriptorHeaps
			auto newHeapIt = m_availableDescriptorHeaps.insert(m_descriptorHeapPool.size() - 1);

			// Use the new manager to allocate the requested size
			allocation = m_descriptorHeapPool[*newHeapIt.first].Allocate(count);
		}

		m_currentSize += (allocation.GetCpuHandle().ptr != 0) ? count : 0;
		m_maxHeapSize = std::max(m_maxHeapSize, m_currentSize);

		return allocation;
	}

	void CpuDescriptorHeap::Free(DescriptorHeapAllocation&& allocation)
	{
		// Retrieve the manager id from the allocation
		auto managerId = allocation.GetManagerId();

		// Update the size of the heap
		m_currentSize -= allocation.GetNumOfHandles();

		// Free the allocation
		m_descriptorHeapPool[managerId].Free(std::move(allocation));
	}

	void CpuDescriptorHeap::ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames)
	{
		// Releases all the stale allocations from the managers of DescriptorHeapPool
		for(size_t i = 0; i < m_descriptorHeapPool.size() ; ++i)
		{
			m_descriptorHeapPool[i].ReleaseStaleAllocations(numOfCompletedFrames);

			// Return the manager to the collection of availableHeaps if it has available descriptors
			if (m_descriptorHeapPool[i].GetAvailableDescriptorCount() > 0)
				m_availableDescriptorHeaps.insert(i);
		}
	}
}