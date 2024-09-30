
#define NOMINMAX


#include "GpuDescriptorHeap.h"

#include "D3DUtillity.h"


namespace BINDU
{
	GpuDescriptorHeap::GpuDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType) :
		m_d3dDevice(pDevice)
	{
		if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			THROW_EXCEPTION(3, "Invalid Descriptor Type");

		m_descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		m_descriptorHeapDesc.NodeMask = 0;
		m_descriptorHeapDesc.NumDescriptors = GPU_DESCRIPTOR_HEAP_DESCRIPTORS_COUNT;
		m_descriptorHeapDesc.Type = heapType;

	}

	GpuDescriptorHeap::~GpuDescriptorHeap()
	{
	}

	void GpuDescriptorHeap::Initialize()
	{
		// Instantiating descriptor heap
		m_descriptorHeap = std::make_shared<DescriptorHeap>(m_d3dDevice, m_descriptorHeapDesc);

		// Instantiating two heap managers
		m_staticHeapManager = std::make_unique<DescriptorHeapManager>(shared_from_this(), m_d3dDevice, m_descriptorHeap,
			m_staticHeapManagerId, m_staticHeapManagerOffset, m_staticHeapManagerDescriptorCount);

		m_dynamicHeapManager = std::make_unique<DescriptorHeapManager>(shared_from_this(), m_d3dDevice, m_descriptorHeap,
			m_dynamicHeapManagerId, m_dynamicHeapManagerOffset, m_dynamicHeapManagerDescriptorCount);
	}

	DescriptorHeapAllocation GpuDescriptorHeap::Allocate(size_t count)
	{
		auto allocation = m_staticHeapManager->Allocate(count);

		m_currentSize += count;

		return allocation;
	}

	DescriptorHeapAllocation GpuDescriptorHeap::AllocateDynamic(size_t count)
	{
		auto allocation = m_dynamicHeapManager->Allocate(count);

		m_currentSize += count;

		return allocation;
	}

	void GpuDescriptorHeap::Free(DescriptorHeapAllocation&& allocation)
	{
		auto managerId = allocation.GetManagerId();

		m_currentSize -= allocation.GetNumOfHandles();

		if(managerId == m_staticHeapManagerId)
		{
			m_staticHeapManager->Free(std::move(allocation));
		}
		else
		{
			m_dynamicHeapManager->Free(std::move(allocation));
		}

	}

	void GpuDescriptorHeap::ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames)
	{
		m_staticHeapManager->ReleaseStaleAllocations(numOfCompletedFrames);
		m_dynamicHeapManager->ReleaseStaleAllocations(numOfCompletedFrames);
	}

	DescriptorHeap* GpuDescriptorHeap::GetDescriptorHeap() const
	{
		return m_descriptorHeap.get();
	}


	// DynamicSubAllocationManager class implementation


	DynamicSubAllocationManager::DynamicSubAllocationManager(GpuDescriptorHeap* parentGpuDescHeap) : m_parentGpuHeap(parentGpuDescHeap)
	{
	}

	DynamicSubAllocationManager::~DynamicSubAllocationManager()
	{
	}

	void DynamicSubAllocationManager::Initialize()
	{
	}

	DescriptorHeapAllocation DynamicSubAllocationManager::Allocate(size_t count)
	{
		// Check if there are no allocations or the last allocation does not have enough space
		if(m_subAllocations.empty() || m_currentSubAllocationOffset + count > m_subAllocations.back().GetNumOfHandles())
		{
			// Request new allocation from Gpu Descriptor heap
			auto subAllocationSize = std::max(m_dynamicChunkSize, count);

			auto newAllocation = m_parentGpuHeap->AllocateDynamic(subAllocationSize);

			m_subAllocations.emplace_back(std::move(newAllocation));

			m_currentSubAllocationOffset = 0;
		}

		// Get the last sub allocation
		auto& lastAllocation = m_subAllocations.back();

		auto managerId = lastAllocation.GetManagerId();

		DescriptorHeapAllocation allocation(nullptr, lastAllocation.GetDescriptorHeap().lock(),
			lastAllocation.GetCpuHandle(m_currentSubAllocationOffset),
			lastAllocation.GetGpuHandle(m_currentSubAllocationOffset),
			count,
			managerId);

		m_currentSubAllocationOffset += count;

		return allocation;
	}

	void DynamicSubAllocationManager::Free(DescriptorHeapAllocation&& allocation)
	{
	}

	void DynamicSubAllocationManager::ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames)
	{
	}

	void DynamicSubAllocationManager::DiscardAllocations(std::uint64_t frameNumber)
	{
		m_subAllocations.clear();
		// Calling the destructor on the vector destroys the DescriptorHeapAllocations which calls its destructor which in turn
		// calls Free() on its parent allocator which will place those allocations in the release queue.
	}
}
