

#include "DescriptorHeapManager.h"

namespace BINDU
{

	// DescriptorHeapAllocation

	DescriptorHeapAllocation::DescriptorHeapAllocation(const std::shared_ptr<DescriptorHeap>& pDescriptorHeap,
	                                                   D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	                                                   D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	                                                   size_t numOfHandles, std::uint32_t managerId) :
		m_descriptorHeap(pDescriptorHeap),
		m_firstCpuHandle(cpuHandle),
		m_firstGpuHandle(gpuHandle),
		m_numOfHandles(numOfHandles), m_managerId(managerId)
	{
		auto descriptorHeap = m_descriptorHeap.lock();

		if (descriptorHeap != nullptr)
			m_descriptorIncrementSize = descriptorHeap->IncrementSize();
	}

	DescriptorHeapAllocation::~DescriptorHeapAllocation()
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapAllocation::GetCpuHandle(std::uint32_t offset) const
	{
		auto cpuHandle = m_firstCpuHandle;
		if (offset != 0)
			cpuHandle.ptr += static_cast<size_t>(offset) * m_descriptorIncrementSize;

		return cpuHandle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapAllocation::GetGpuHandle(std::uint32_t offset) const
	{
		auto gpuHandle = m_firstGpuHandle;
		if (offset != 0)
			gpuHandle.ptr += static_cast<size_t>(offset) * m_descriptorIncrementSize;

		return gpuHandle;
	}

	size_t DescriptorHeapAllocation::GetNumOfHandles() const
	{
		return m_numOfHandles;
	}

	bool DescriptorHeapAllocation::IsNull() const
	{
		return m_firstCpuHandle.ptr == 0;
	}

	bool DescriptorHeapAllocation::IsShaderVisible() const
	{
		return m_firstGpuHandle.ptr != 0;
	}

	std::uint32_t DescriptorHeapAllocation::GetManagerId() const
	{
		return m_managerId;
	}
	std::uint32_t DescriptorHeapAllocation::GetDescriptorIncrementSize() const
	{
		return m_descriptorIncrementSize;
	}





	
	// DescriptorHeapManager


	DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, std::uint32_t managerId) :
		m_d3dDevice(pDevice), m_freeMemAllocator(heapDesc.NumDescriptors), m_managerId(managerId)
	{
		m_descriptorHeap = std::make_shared<DescriptorHeap>(m_d3dDevice, heapDesc.Type, heapDesc.Flags, heapDesc.NumDescriptors);

		m_firstCpuHandle = m_descriptorHeap->GetFirstCPUHandle();

		m_firstGpuHandle = m_descriptorHeap->GetFirstGPUHandle();

		m_freeDescriptors = heapDesc.NumDescriptors;
	}

	DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* pDevice, DescriptorHeap* pDescriptorHeap, std::uint32_t managerId,
		std::uint32_t offset, size_t numOfDescriptors) :
		m_d3dDevice(pDevice), m_descriptorHeap(pDescriptorHeap), m_freeMemAllocator(numOfDescriptors), m_managerId(managerId)
	{
		m_firstCpuHandle = m_descriptorHeap->GetCPUHandleAt(offset);

		m_firstGpuHandle = m_descriptorHeap->GetGPUHandleAt(offset);

		m_freeDescriptors = numOfDescriptors;
	}

	DescriptorHeapManager::~DescriptorHeapManager()
	{
	}

	DescriptorHeapAllocation DescriptorHeapManager::Allocate(size_t count)
	{
		// Use the variable size memory allocator to allocate the requested amount of memory
		auto descriptorHandleOffset = m_freeMemAllocator.Allocate(count);

		if (descriptorHandleOffset == InvalidOffset)
			return {};

		// Compute the first cpu and gpu descriptor handle in the heap using the offset
		auto cpuHandle = m_firstCpuHandle;
		cpuHandle.ptr += static_cast<size_t>(descriptorHandleOffset) * m_descriptorHeap->IncrementSize();

		auto gpuHandle = m_firstGpuHandle;
		if (m_descriptorHeap->Flags() & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
			gpuHandle.ptr += static_cast<size_t>(descriptorHandleOffset) * m_descriptorHeap->IncrementSize();

		// Decrease the free descriptor count
		m_freeDescriptors -= count;

		return { m_descriptorHeap, cpuHandle, gpuHandle, count, m_managerId };
	}

	void DescriptorHeapManager::Free(DescriptorHeapAllocation& allocation)
	{
		auto descriptorOffset = (allocation.GetCpuHandle().ptr - m_firstCpuHandle.ptr) / m_descriptorHeap->IncrementSize();

		// The allocation is not released immediately. It gets added to the release queue in the VariableSizeGpuMemoryAllocator
		m_freeMemAllocator.Free(static_cast<std::uint32_t>(descriptorOffset), allocation.GetNumOfHandles(), 1);

		// Increase the free descriptor count
		m_freeDescriptors += allocation.GetNumOfHandles();

		// Clear the allocation
		allocation = DescriptorHeapAllocation();
	}

	void DescriptorHeapManager::ReleaseStaleAllocations(std::uint64_t numOfCompletedFrames)
	{
		m_freeMemAllocator.ReleaseCompletedFrames(numOfCompletedFrames);
	}

	size_t DescriptorHeapManager::GetAvailableDescriptorCount() const
	{
		return m_freeDescriptors;
	}
}