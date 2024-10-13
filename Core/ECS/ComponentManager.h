#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "IComponent.h"

namespace BINDU
{
	using EntityId = std::uint32_t;

	class ComponentManager
	{
	public:
		ComponentManager() = default;
		~ComponentManager() = default;

		template<class ComponentType>
		void	RegisterComponent()
		{
			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			if (m_componentSignatures.find(typeIndex) == m_componentSignatures.end())
			{

				auto size = m_componentSignatures.size();
				EntitySig sig = 1 << size;
				m_componentSignatures[typeIndex] = sig;
			}

		}

		template<class ComponentType>
		void	UnregisterComponent()
		{
			/// TODO: Possible room for improvement(Simplifying by only removing the components signature from entitySignatures)///

			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			auto sig = m_componentSignatures[typeIndex];

			for (auto& entity : m_components)
			{
				entity.second.erase(sig);
				m_entitySignatures[entity.first] &= ~sig;
			}

			m_componentSignatures.erase(typeIndex);
		}

		// Add a component to an Entity
		template<class ComponentType>
		void	AddComponent(EntityId entity)
		{
/*			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			// if there is no component vector for this type, create one
			if (m_components[typeIndex].find(entity) == m_components[typeIndex].end())
			{
				m_components[typeIndex].emplace(std::make_unique<ComponentType>(std::forward<Args>(args)...));
			} */


			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			auto sig = m_componentSignatures[typeIndex];

			if (m_entitySignatures[entity] & sig)
				return;

			m_entitySignatures[entity] |= sig;

			auto itr = m_components[entity].emplace(sig, std::make_unique<ComponentType>());
			itr.first->second->Signature = sig;
		}

		// Get component of an specific type for an Entity
		template<class ComponentType>
		ComponentType* GetComponent(EntityId entity)
		{
		/*	std::type_index typeIndex = std::type_index(typeid(ComponentType));
			
			if (m_components.find(typeIndex) != m_components.end() &&
				m_components[typeIndex].find(entity) != m_components[typeIndex].end())
			{
				return std::dynamic_pointer_cast<ComponentType>(m_components[typeIndex][entity]);
			}
			return nullptr;

			auto sig = ComponentType::Sig; */

			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			auto sig = m_componentSignatures[typeIndex];

			if (m_entitySignatures[entity] & sig)
			{
				return static_cast<ComponentType*>(m_components[entity][sig].get());
			}
			
			return nullptr;
		}

		// Check if an Entity has a component of a specific type
		template<class ComponentType>
		bool		HasComponent(EntityId entity)
		{
		/*	std::type_index typeIndex = std::type_index(typeid(ComponentType));
			return m_components.find(typeIndex) != m_components.end() &&
				m_components[typeIndex].find(entity) != m_components[typeIndex].end(); */

			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			auto sig = m_componentSignatures[typeIndex];

			return m_entitySignatures[entity] & sig;
		}

		// Remove component of a specific type from an Entity
		template<class ComponentType>
		void		RemoveComponent(EntityId entity)
		{
			/* std::type_index typeIndex = std::type_index(typeid(ComponentType));
			m_components[typeIndex].erase(entity); */

			std::type_index typeIndex = std::type_index(typeid(ComponentType));

			auto sig = m_componentSignatures[typeIndex];

			m_entitySignatures[entity] &= ~sig;
			m_components[entity].erase(sig);
		}

		void		RemoveEntity(EntityId entity)
		{
			m_components.erase(entity);
			m_entitySignatures.erase(entity);
		}

	private:
	//	std::unordered_map<std::type_index, std::unordered_map<EntityId, std::unique_ptr<IComponent>>>		m_components;
		
		using ComponentsMap = std::unordered_map<EntityId, std::unordered_map<EntitySig, std::unique_ptr<IComponent>>>;

		std::unordered_map<std::type_index, EntitySig>										m_componentSignatures;
		ComponentsMap																		m_components;
		std::unordered_map<EntityId, EntitySig>												m_entitySignatures;
	};
}

#endif