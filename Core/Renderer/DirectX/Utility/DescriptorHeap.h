#ifndef DESCRIPTORHEAP_H
#define DESCRIPTORHEAP_H

#include <d3d12.h>
#include <cstdint>
#include <memory>
#include <wrl/client.h>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class DescriptorHeap
	{
	public:
		DescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags, std::uint32_t count);

		// No copy constructor
		DescriptorHeap(const DescriptorHeap& rhs) = delete;

		// No copy assignment
		DescriptorHeap& operator = (const DescriptorHeap& rhs) = delete;

		// No move constructor
		DescriptorHeap(const DescriptorHeap&& rhs) = delete;

		// No move assignment
		DescriptorHeap& operator = (const DescriptorHeap&& rhs) = delete;

		~DescriptorHeap();


		// Get the CPU handle to the first descriptor in the heap
		D3D12_CPU_DESCRIPTOR_HANDLE		GetFirstCPUHandle() const;

		// Get the GPU handle to the first descriptor in the heap
		D3D12_GPU_DESCRIPTOR_HANDLE		GetFirstGPUHandle() const;

		// Get the CPU handle to the descriptor at index
		D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandleAt(std::uint32_t index) const;

		// Get the GPU handle to the descriptor at index
		D3D12_GPU_DESCRIPTOR_HANDLE		GetGPUHandleAt(std::uint32_t index) const;

		// Returns the count of the descriptors in the heap
		std::uint32_t					Count() const;

		// Return the type of the descriptor heap
		D3D12_DESCRIPTOR_HEAP_TYPE		GetType() const;

		// Returns the descriptor handle increment size for this descriptor heap
		std::uint32_t					IncrementSize() const;

		// Returns the underlying heap
		ComPtr<ID3D12DescriptorHeap>&	Heap() const;

		// Returns the heap is null or not
		bool							IsNull() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl{ nullptr };

	};
}


#endif