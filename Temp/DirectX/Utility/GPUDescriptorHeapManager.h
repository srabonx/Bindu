#ifndef GPUDESCRIPTORHEAPMANAGER_H
#define GPUDESCRIPTORHEAPMANAGER_H

#include <map>

#include "DescriptorHeap.h"
#include "../../../../Utility/Common/CircleBuffer.h"

namespace BINDU
{
	class GPUDescriptorHeapManager
	{
	public:
		explicit GPUDescriptorHeapManager(DescriptorHeap* gpuHeapToManage);

		~GPUDescriptorHeapManager();

		void	CopyDescriptor(ID3D12Device* pDevice, DescriptorHeap* src, std::uint32_t count);

		void	CopyDescriptor(ID3D12Device* pDevice, std::uint32_t destOffset, DescriptorHeap* src, std::uint32_t count);

		void	Reset();

	private:
		DescriptorHeap*										m_descriptorHeap{ nullptr };
		std::uint32_t										m_currOffset{ 0 };
		std::uint32_t										m_freeSize{ 0 };
	};
}

#endif