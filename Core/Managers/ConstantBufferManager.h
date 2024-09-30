#ifndef CONSTANTBUFFERMANAGER_H
#define CONSTANTBUFFERMANAGER_H
#include <set>

#include "../Renderer/DirectX/D3DConstantBuffer.h"

namespace BINDU
{

	class ConstantBufferManager
	{
	public:
		ConstantBufferManager(std::uint64_t defaultBufferSize, std::uint32_t cbDataByteSize);

		~ConstantBufferManager() = default;

		void				Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

	/*	CBAllocation		Allocate(std::uint64_t size = 1)
		{
			CBAllocation allocation;
			allocation.CBIndex = UINT64_MAX;

			for (auto itr = m_availableBuffers.begin(); itr != m_availableBuffers.end(); ++itr)
			{
				allocation = m_constantBufferPool[*itr]->Allocate(size);

				if (m_constantBufferPool[*itr]->GetFreeSize() == 0)
					m_availableBuffers.erase(*itr);

				if (allocation.CBIndex != UINT64_MAX)
					break;
			}

			if (allocation.CBIndex == UINT64_MAX)
			{
				auto newSize = m_defaultBufferSize;//std::max(m_defaultBufferSize, size);

				auto bufferId = static_cast<std::uint16_t>(m_constantBufferPool.size());

				m_constantBufferPool.emplace_back(std::make_shared<D3DConstantBuffer>(newSize, bufferId));

				m_constantBufferPool[bufferId]->Initialize(m_deviceManager.get(), m_cbDataByteSize);

				m_availableBuffers.insert(bufferId);

				allocation = m_constantBufferPool[bufferId]->Allocate(size);
			}

			m_currentSize += (allocation.CBIndex == UINT64_MAX) ? 0 : size;

			//m_maxSize = std::max(m_maxSize, m_currentSize);

			return allocation;
		} 

		void				Free(const CBAllocation& allocation);

		template <typename T>
		void				UpdateConstantBuffer(const CBAllocation& allocation, const T& data)
		{
			auto bufferIndex = allocation.BufferId;
			auto CBindex = allocation.CBIndex;

			m_constantBufferPool[bufferIndex]->UpdateBuffer(CBindex, data);
		} */

		std::shared_ptr<D3DConstantBuffer>& GetBuffer(std::uint64_t bufferId);

		std::uint64_t		GetCurrentSize() const;

		std::uint64_t		GetMaxSize() const;

	private:

		std::shared_ptr<D3DDeviceManager>							m_deviceManager{ nullptr };

		std::vector<std::shared_ptr<D3DConstantBuffer>>				m_constantBufferPool;

		std::set<std::uint64_t>										m_availableBuffers;

		std::uint64_t												m_currentSize{ 0 };

		std::uint64_t												m_maxSize{ 0 };

		std::uint64_t												m_defaultBufferSize{ 0 };

		std::uint32_t												m_cbDataByteSize{ 0 };
	};
}



#endif