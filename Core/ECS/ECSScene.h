#ifndef ECSSCENE_H
#define ECSSCENE_H

#include "EntityManager.h"
#include "ComponentRegistry.h"

namespace BINDU
{
	class Entity;

	class ECSScene
	{
		friend class Entity;

	public:
		ECSScene() = default;
		~ECSScene() = default;

		Entity	CreateEntity(const std::string& name = std::string());

		void	DestroyEntity(const Entity* entity);

		void	DestroyEntity(EntityId entity);

		void	OnUpdate(double dt);

	private:
		EntityManager		m_entityManager;
		ComponentRegistry	m_componentRegistry;
	};
}

#endif