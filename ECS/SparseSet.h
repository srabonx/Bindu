#ifndef SPARSESET_H
#define SPARSESET_H

#include <vector>

#include "ECSConstants.h"
#include "../Debug/Assert.h"
#include "../Utility/Common/CommonUtility.h"

namespace BINDU
{

	class ISparseSet
	{
	public:
		virtual ~ISparseSet() = default;
		virtual void	Remove(EntityId entity) = 0;
	};

	template<typename Component>
	class SparseSet : public ISparseSet
	{
	public:
		SparseSet() = default;
		~SparseSet() override = default;

		Component&	Add(EntityId entity,const Component& component)
		{
			if (entity >= m_sparsePool.size())
			{
				m_sparsePool.resize(entity + 1, InvalidIndex);
			}

			BINDU_CORE_ASSERT(m_sparsePool[entity] == InvalidIndex, "Entity already has the component!");

			m_sparsePool[entity] = m_densePool.size();

			m_densePool.emplace_back(entity);
			
			return m_componentPool.emplace_back(component);
		}

		void	Remove(EntityId entity) override
		{
			BINDU_CORE_ASSERT(entity < m_sparsePool.size(), "Entity out of range!");

			auto denseIndex = m_sparsePool[entity];

			BINDU_CORE_ASSERT(denseIndex != InvalidIndex, "Entity doesn't have the component!");

			// Swap the last element with the one to be removed
			auto lastEntity = m_densePool.back();

			std::swap(m_densePool[denseIndex], m_densePool.back());

			std::swap(m_componentPool[denseIndex], m_componentPool.back());

			m_sparsePool[lastEntity] = denseIndex;

			m_sparsePool[entity] = InvalidIndex;


			// Pop the last elements
			m_densePool.pop_back();
			m_componentPool.pop_back();
		}

		bool	Has(EntityId entity) const
		{
			return entity < m_sparsePool.size() && m_sparsePool[entity] != InvalidIndex;
		}

		Component& Get(EntityId entity)
		{
			BINDU_CORE_ASSERT(Has(entity), "Entity doesn't have the component!");

			return m_componentPool[m_sparsePool[entity]];
		}

		// Iterate over all entities with the same component
		// syntax : Each([](EntityId entity, Component& component);
		template<typename Func>
		void	Each(Func func)
		{
			for (auto i = 0; i < m_densePool.size(); i++)
			{
				func(m_densePool[i], m_componentPool[i]);
			}
		}

		const std::vector<Component>& GetComponents() const { return m_componentPool; }
		const std::vector<EntityId>& GetDenseEntities() const { return m_densePool; }
		const std::vector<EntityId>& GetSparseEntities() const { return m_sparsePool; }

	private:
	
		std::vector<EntityId>		m_sparsePool;
		std::vector<EntityId>		m_densePool;
		std::vector<Component>		m_componentPool;
	};
}

#endif