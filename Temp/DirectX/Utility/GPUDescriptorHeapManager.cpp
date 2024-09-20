#include "GPUDescriptorHeapManager.h"

#include "../../../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	BINDU::GPUDescriptorHeapManager::GPUDescriptorHeapManager(DescriptorHeap* gpuHeapToManage) : m_descriptorHeap(gpuHeapToManage)
	{
		if((gpuHeapToManage->Flags() & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) != D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		{
			THROW_EXCEPTION(3, "Descriptor heap must be shader visible for GPUDescriptorHeapManager to manage");
		}

		m_freeSize = gpuHeapToManage->Count();
	}

	GPUDescriptorHeapManager::~GPUDescriptorHeapManager()
	{
	}

	void GPUDescriptorHeapManager::CopyDescriptor(ID3D12Device* pDevice, DescriptorHeap* src, std::uint32_t count)
	{
		if(src->Type() != m_descriptorHeap->Type())
		{
			THROW_EXCEPTION(3, "Descriptor heap types must match for copying");
		}


		if (count <= m_freeSize)
		{
			pDevice->CopyDescriptorsSimple(count, m_descriptorHeap->GetCPUHandleAt(m_currOffset),
				src->GetFirstCPUHandle(), src->Type());

			m_currOffset += count;

			m_freeSize -= count;
		}
		else
		{
			THROW_EXCEPTION(3, "Not enough free space to copy in GPU descriptor");
		}
	}

	void GPUDescriptorHeapManager::CopyDescriptor(ID3D12Device* pDevice, std::uint32_t destOffset, DescriptorHeap* src,
		std::uint32_t count)
	{
		if (src->Type() != m_descriptorHeap->Type())
		{
			THROW_EXCEPTION(3, "Descriptor heap types must match for copying");
		}

		auto freeSize = m_descriptorHeap->Count() - destOffset;


		if (count <= freeSize)
		{
			pDevice->CopyDescriptorsSimple(count, m_descriptorHeap->GetCPUHandleAt(destOffset),
				src->GetFirstCPUHandle(), src->Type());

			auto heapOffset = destOffset + count;

			if (heapOffset > m_currOffset)
			{
				m_currOffset = heapOffset;

				m_freeSize = m_descriptorHeap->Count() - m_currOffset;
			}
		}
		else
		{
			THROW_EXCEPTION(3, "Not enough free space to copy in GPU descriptor");
		}
	}

	void GPUDescriptorHeapManager::Reset()
	{
		m_currOffset = 0;
		m_freeSize = m_descriptorHeap->Count();
	}
}
