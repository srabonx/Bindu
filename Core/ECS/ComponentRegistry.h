#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include "SparseSet.h"
#include <unordered_map>
#include <typeindex>
#include <any>

#include "View.h"

namespace BINDU
{

	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;
		~ComponentRegistry() = default;

		template<typename Component, typename... Args>
		Component& AddComponent(EntityId entity, Args&&... args)
		{
			auto& set = GetOrCreateSparseSet<Component>();
		
			Component component(std::forward<Args>(args)...);

			return set.Add(entity, component);
		} 

		// Get component for an entity
		template<typename Component>
		Component& GetComponent(EntityId entity)
		{
			return GetOrCreateSparseSet<Component>().Get(entity);
		}

		// Check if an entity has a component
		template<typename Component>
		bool		HasComponent(EntityId entity)
		{
			return m_sparseSets.count(std::type_index(typeid(Component))) && GetSparseSet<Component>().Has(entity);
		}

		// Remove component from an entity
		template<typename Component>
		void	RemoveComponent(EntityId entity)
		{
			if (HasComponent<Component>(entity))
			{
				GetSparseSet<Component>().Remove(entity);
			}
		}

		template<typename... Components>
		View<Components...>	GetView()
		{
			return View<Components...>(this);
		}

		void	RemoveEntity(EntityId entity) const
		{
			for (auto& s : m_sparseSets)
			{
				s.second->Remove(entity);
			}
		} 

		// Get the sparse set for a component Type
		template<typename Component>
		const SparseSet<Component>& GetSparseSet() const
		{
			auto typeIndex = std::type_index(typeid(Component));
			
			auto sparseSet = m_sparseSets.at(typeIndex).get();
			return static_cast<const SparseSet<Component>&>(*sparseSet);
		}

		template<typename Component>
		SparseSet<Component>& GetOrCreateSparseSet()
		{
			auto typeIndex = std::type_index(typeid(Component));
			
			if (!m_sparseSets.count(typeIndex))
			{
				m_sparseSets.emplace(typeIndex, std::make_unique<SparseSet<Component>>());
			}

			return static_cast<SparseSet<Component>&>(*m_sparseSets[typeIndex]);
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>>		m_sparseSets;
	};
}

#endif