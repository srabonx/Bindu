#ifndef ECSVIEW_H
#define ECSVIEW_H

#include <tuple>
#include <variant>
#include <functional>
#include <type_traits>

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
			// Find the index of the component in the tuple
			constexpr auto componentIndex = GetComponentIndex<Component>();

			// Get the entities associated with the component
			const auto* entities = [componentIndex];

			// Check if the entity vector is valid
			if (!entities)
			{
				throw std::runtime_error("Entities vector is null");
			}

			// Get the component vector for this component type
			const auto* components = static_cast<const std::vector<Component>*>(m_componentArray[componentIndex]);

			// Find the entity's position in the entities vector
			EntityId pos = entities->at(entity);//std::find(entities->begin(), entities->end(), entity);

			// Ensure the entity was found in the entities vector
			if (pos == InvalidIndex)
			{
				throw std::runtime_error("Entity not found");
			}  

			// Calculate the index of the entity and return the corresponding component
			//size_t index = pos - entities->begin();
			return (*components)[pos];
		}

	private:

		template<size_t I = 0, typename... Types>
		typename std::tuple_element<I, std::tuple<Types...>>::type& GetTupleElement(std::tuple<Types...>& tuple, size_t index)
		{
			if constexpr (I < sizeof...(Types))
			{
				if (I == index)
				{
					return std::get<I>(tuple);
				}
				else
				{
					return GetTupleElement<I + 1>(tuple, index);
				}

			}
			else
			{
				throw std::out_of_range("Index is out of range");

			}
		}

		template<typename TupleType, typename Func, size_t... Is>
		void	SetTupleElementImpl(TupleType& tuple, size_t index, Func func, std::index_sequence<Is...>)
		{
			(..., (index == Is ? func(std::get<Is>(tuple)) : void()));
		}

		template<typename TupleType, typename Func>
		void	SetTupleElement(TupleType& tuple, size_t index, Func func)
		{
			constexpr size_t tupleSize = std::tuple_size_v<TupleType>;
			SetTupleElementImpl(tuple, index, func, std::make_index_sequence<tupleSize>{});
		}

		template<typename Component, typename... Rest>
		void	SetupViewRecursive(size_t i = 0)
		{
			size_t index = sizeof...(Rest);

			m_entities[i] = &m_componentRegistry->GetOrCreateSparseSet<Component>().GetDenseEntities();

			m_componentArray[i] = static_cast<const void*>(&m_componentRegistry->GetSparseSet<Component>().GetComponents());

		/*	SetTupleElement(m_components, index, [&](auto& componentVector)
				{
					using ComponentType = typename std::remove_reference_t<decltype(componentVector)>::value_type;
					componentVector = m_componentRegistry->GetOrCreateSparseSet<ComponentType>().GetComponents();
				});
				 */

			if constexpr (sizeof...(Rest) > 0)
			{
				SetupViewRecursive<Rest...>(i + 1);
			}
		}

		void	SetupView()
		{
		//	SetupViewRecursive<Components...>();


			size_t i = 0;
			((m_entities[i] = &m_componentRegistry->GetOrCreateSparseSet<Components>().GetDenseEntities(),
				m_componentArray[i] = static_cast<const void*>(&m_componentRegistry->GetSparseSet<Components>().GetComponents()),
				++i), ...);

			size_t index = 0;
			((m_sparseSets[i] = static_cast<const void*>(& m_componentRegistry->GetOrCreateSparseSet<Components>()), ++index), ...);

			size_t min_size = static_cast<size_t>(-1);
			size_t min_index = 0;
			for (size_t j = 0; j < sizeof...(Components); ++j) {

				auto size = static_cast<SparseSet<Components...>*>(m_sparseSets[j])->GetDenseEntities().size();
				if ( size < min_size) {
					min_size = size;
					min_index = j;
				}
			}
			if (min_index != 0) {
				std::swap(m_sparseSets[0], m_sparseSets[min_index]);
			}

/*			size_t min_size = static_cast<size_t>(-1);
			size_t min_index = 0;
			for (size_t j = 0; j < sizeof...(Components); ++j) {
				if (m_entities[j]->size() < min_size) {
					min_size = m_entities[j]->size();
					min_index = j;
				}
			}
			if (min_index != 0) {
				std::swap(m_entities[0], m_entities[min_index]);
				std::swap(m_componentArray[0], m_componentArray[min_index]);
			} */


		}

		// Helper to find index of a component in the tuple
		template<typename Component>
		static constexpr size_t GetComponentIndex()
		{
			return FindComponentIndex<Component, Components...>();
		}

		// Compile-time recursive function to find component index in the template pack
		template<typename Target, typename First, typename... Rest>
		static constexpr size_t FindComponentIndex(size_t index = 0)
		{
			if constexpr (std::is_same_v<Target, First>)
			{
				return index;
			}
			else
			{
				return FindComponentIndex<Target, Rest...>(index + 1);
			}
		}

	private:
		ComponentRegistry*												m_componentRegistry{ nullptr };

		std::array<const std::vector<EntityId>*, sizeof...(Components)>	m_entities;
		//std::tuple<std::vector<Components>...>							m_components;
		std::array<const void*, sizeof...(Components)>					m_componentArray;

		std::array<const void*, sizeof...(Components)>					m_sparseSets;
	};
}

#endif