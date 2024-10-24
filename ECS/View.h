#ifndef ECSVIEW_H
#define ECSVIEW_H

#include <algorithm>
#include <tuple>
#include <variant>
#include <functional>
#include <type_traits>

#include "ComponentRegistry.h"


namespace BINDU
{
	class ComponentRegistry;

	using ConstViewIterator = const std::vector<EntityId>::const_iterator;

	template<typename... Components>
	class View
	{
	public:
		View(ComponentRegistry* componentRegistry) : m_componentRegistry(componentRegistry) { SetupView(); }

		~View() = default;

		ConstViewIterator begin() const { return m_entities[0]->begin(); }

		ConstViewIterator end() const { return m_entities[0]->end(); }

		ConstViewIterator rbegin() const { return m_entities[0]->rbegin(); }

		ConstViewIterator rend() const { return m_entities[0]->rend(); }

		template<typename... components>
		std::tuple<const components&...> Get(EntityId entity)
		{
			return std::make_tuple(std::ref(GetComponent<components>(entity))...);
		}

		template<typename Component>
		const Component& GetComponent(EntityId entity)
		{
			return m_componentRegistry->GetComponent<Component>(entity);
		}

	private:

		void	SetupView()
		{
			size_t index = 0;
			size_t minSize = static_cast<size_t>(-1);
			size_t minIndex{ 0 };

			((m_entities[index] = &m_componentRegistry->GetOrCreateSparseSet<Components>().GetDenseEntities(),
				++index), ...);


			index = 0;
			std::for_each(m_entities.begin(), m_entities.end(), [&](const std::vector<EntityId>* v)
				{
					if (v->size() < minSize)
					{
						minSize = v->size();
						minIndex = index;

						++index;
					}
				});

			if(minIndex != 0)
			{
				std::swap(m_entities[0], m_entities[minIndex]);
			}

		}

	private:
		ComponentRegistry*													m_componentRegistry{ nullptr };
		std::array<const std::vector<EntityId>*, sizeof...(Components)>		m_entities;

	};
}

#endif