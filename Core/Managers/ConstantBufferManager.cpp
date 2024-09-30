
#define NOMINMAX

#include "ConstantBufferManager.h"

#include "../GameObject/GameObject.h"

namespace BINDU
{
	ConstantBufferManager::ConstantBufferManager(std::uint64_t defaultBufferSize, std::uint32_t cbDataByteSize) : m_defaultBufferSize(defaultBufferSize),
																													m_cbDataByteSize(cbDataByteSize)
	{
		
	}

	void ConstantBufferManager::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager)
	{
		m_deviceManager = deviceManager;
	}

/*	template<typename T>
	CBAllocation ConstantBufferManager<T>::Allocate(std::uint64_t size)
	{
		CBAllocation allocation;
		allocation.CBIndex = UINT64_MAX;

		for(auto itr = m_availableBuffers.begin(); itr != m_availableBuffers.end(); ++itr)
		{
			allocation = m_constantBufferPool[*itr]->Allocate(size);

			if (m_constantBufferPool[*itr]->GetFreeSize() == 0)
				m_availableBuffers.erase(*itr);

			if (allocation.CBIndex != UINT64_MAX)
				break;
		}

		if(allocation.CBIndex == UINT64_MAX)
		{
			auto newSize = std::max(m_defaultBufferSize, size);

			auto bufferId = static_cast<std::uint16_t>(m_constantBufferPool.size());

			m_constantBufferPool.emplace_back(std::make_shared<D3DConstantBuffer<T>>(newSize, bufferId));

			m_constantBufferPool[bufferId]->Initialize(m_deviceManager.get());

			m_availableBuffers.insert(bufferId);

			allocation = m_constantBufferPool[bufferId]->Allocate(size);
		}

		m_currentSize += (allocation.CBIndex == UINT64_MAX) ? 0 : size;

		m_maxSize = std::max(m_maxSize, m_currentSize);

		return allocation;
	} 

	void ConstantBufferManager::Free(const CBAllocation& allocation)
	{
		auto bufferId = allocation.BufferId;

		m_constantBufferPool[bufferId]->Free(allocation.CBIndex, allocation.Size);

		m_currentSize -= allocation.Size;

		if (m_constantBufferPool[bufferId]->GetFreeSize() > 0)
			m_availableBuffers.insert(bufferId);

	} */

	std::shared_ptr<D3DConstantBuffer>& ConstantBufferManager::GetBuffer(std::uint64_t bufferId)
	{
		return m_constantBufferPool[bufferId];
	}

	std::uint64_t ConstantBufferManager::GetCurrentSize() const
	{
		return m_currentSize;
	}

	std::uint64_t ConstantBufferManager::GetMaxSize() const
	{
		return m_maxSize;
	}

}
