#ifndef ECSENTITY_H
#define ECSENTITY_H

#include "ECSScene.h"
#include "../../Utility/Common/CommonUtility.h"

namespace BINDU
{
	class Entity
	{
	public:
		Entity(EntityId entityId, ECSScene* scene);
		
		Entity(const Entity& other) = default;
		
		Entity& operator=(const Entity& other) = default;

		~Entity() = default;

		template<typename Component, typename... Args>
		Component&		AddComponent(Args&&... args)
		{
			BINDU_ASSERT(!HasComponent<Component>(), "Entity already has the component!");

			return m_parentScene->m_componentRegistry.AddComponent<Component>(m_entityId, std::forward<Args>(args)...);
		}

		template<typename Component>
		Component&		GetComponent()
		{
			BINDU_ASSERT(HasComponent<Component>(), "Entity doesn't have the component!");

			return m_parentScene->m_componentRegistry.GetComponent<Component>(m_entityId);
		}

		template<typename Component>
		void			RemoveComponent()
		{
			BINDU_ASSERT(HasComponent<Component>(), "Entity doesn't have the component!");

			m_parentScene->m_componentRegistry.RemoveComponent<Component>(m_entityId);
		}

		template<typename Component>
		bool			HasComponent() const
		{
			return m_parentScene->m_componentRegistry.HasComponent<Component>(m_entityId);
		}

		operator bool() const { return m_parentScene != nullptr; }

		EntityId		GetId() const;

	private:
		EntityId				m_entityId{ 0 };
		ECSScene*				m_parentScene{ nullptr };

	};
}

#endif