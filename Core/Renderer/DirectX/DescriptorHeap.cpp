#include "DescriptorHeap.h"

#include <map>
#include <wrl/client.h>

#include "d3dx12.h"
#include "../../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	using offset = std::uint32_t;
	using size = std::uint32_t;

	class DescriptorHeap::Impl
	{
	public:
		Impl(D3D12_DESCRIPTOR_HEAP_TYPE heapType,
			D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags, std::uint32_t count) : m_type(heapType), m_flags(heapFlags), m_count(count) {}

		~Impl() {}

		// The underlying heap
		ComPtr<ID3D12DescriptorHeap>		m_heap{ nullptr };

		// Heap type
		D3D12_DESCRIPTOR_HEAP_TYPE			m_type;

		// Heap flags
		D3D12_DESCRIPTOR_HEAP_FLAGS			m_flags;

		// Descriptor count
		std::uint32_t						m_count{ 0 };

		// Handle increment size
		std::uint32_t						m_incrementSize{ 0 };


	};


	DescriptorHeap::DescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags, std::uint32_t count) : m_impl(std::make_unique<Impl>(heapType, heapFlags, count))
	{
		if (!pDevice)
			THROW_EXCEPTION(3, "Invalid Device");
		
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = heapType;
		heapDesc.NodeMask = 0;
		heapDesc.Flags = heapFlags;
		heapDesc.NumDescriptors = count;

		m_impl->m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(heapType);

		pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_impl->m_heap.ReleaseAndGetAddressOf()));

	}

	DescriptorHeap::DescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_DESC heapDesc) :
		m_impl(std::make_unique<Impl>(heapDesc.Type, heapDesc.Flags, heapDesc.NumDescriptors))
	{
		if (!pDevice)
			THROW_EXCEPTION(3, "Invalid Device");

		m_impl->m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(heapDesc.Type);

		pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_impl->m_heap.ReleaseAndGetAddressOf()));
	}

	DescriptorHeap::~DescriptorHeap()
	{

	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetFirstCPUHandle() const
	{
		return m_impl->m_heap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetFirstGPUHandle() const
	{
		if (m_impl->m_flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
			return m_impl->m_heap->GetGPUDescriptorHandleForHeapStart();

		return CD3DX12_GPU_DESCRIPTOR_HANDLE();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandleAt(std::uint32_t index) const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_impl->m_heap->GetCPUDescriptorHandleForHeapStart(),
		                                     static_cast<INT>(index), m_impl->m_incrementSize);
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandleAt(std::uint32_t index) const
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_impl->m_heap->GetGPUDescriptorHandleForHeapStart(),
											 static_cast<INT>(index), m_impl->m_incrementSize);
	}

	std::uint32_t DescriptorHeap::Count() const
	{
		return m_impl->m_count;
	}

	D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::Type() const
	{
		return m_impl->m_type;
	}

	D3D12_DESCRIPTOR_HEAP_FLAGS DescriptorHeap::Flags() const
	{
		return m_impl->m_flags;
	}

	std::uint32_t DescriptorHeap::IncrementSize() const
	{
		return m_impl->m_incrementSize;
	}
	ComPtr<ID3D12DescriptorHeap>& DescriptorHeap::Heap() const
	{
		return m_impl->m_heap;
	}

	bool DescriptorHeap::IsNull() const
	{
		return m_impl->m_count == 0;
	}
}
