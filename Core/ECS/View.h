#ifndef ECSVIEW_H
#define ECSVIEW_H

#include <algorithm>
#include <tuple>
#include <variant>
#include <functional>
#include <type_traits>

#include "ComponentRegistry.h"
#include "ComponentRegistry.h"
#include "ComponentRegistry.h"

namespace BINDU
{
	class ComponentRegistry;

	template<typename... Components>
	class View
	{
	public:
		View(ComponentRegistry* componentRegistry) : m_componentRegistry(componentRegistry) { SetupView(); }

		~View() = default;

		class Iterator
		{
		public:
			Iterator(const View& view, size_t index) : m_view(view), m_index(index) {}

			EntityId operator*()
			{
				return m_view.m_entities[0]->at(m_index);
			}

			Iterator& operator++()
			{
				++m_index;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}

			bool operator !=(const Iterator& other) const
			{
				return m_index != other.m_index;
			}

		private:
			const View& m_view;
			size_t		m_index;
		};

		Iterator Begin() const { return Iterator(*this, 0); }

		Iterator End() const { return Iterator(*this, m_entities[0]->size()); }

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