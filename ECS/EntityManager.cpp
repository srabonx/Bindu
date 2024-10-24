#include "EntityManager.h"

namespace BINDU
{
	EntityId EntityManager::CreateEntity()
	{
		auto entityId = static_cast<EntityId>(m_entityAllocator.Allocate(1));
		m_entities.emplace(entityId);
		return entityId;
	}

	void EntityManager::RemoveEntity(EntityId entity)
	{
		if (HasEntity(entity))
		{
			m_entities.erase(entity);
			m_entityAllocator.Free(entity, 1);
		}
	}

	bool EntityManager::HasEntity(EntityId entity)
	{
		return m_entities.find(entity) != m_entities.end();
	}
}
