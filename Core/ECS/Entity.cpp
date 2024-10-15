#include "Entity.h"

namespace BINDU
{
	Entity::Entity(EntityId entityId, ECSScene* scene) :	m_entityId(entityId), m_parentScene(scene)
	{
	}

	EntityId Entity::GetId() const
	{
		return m_entityId;
	}
}
