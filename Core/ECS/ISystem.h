#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <cstdint>
#include <unordered_set>

namespace BINDU
{
	using EntityId = std::uint32_t;

	class ISystem
	{
	public:
		virtual ~ISystem() {}

		// This function needs to be overriden by each specific system to update Entities
		virtual void	Update(double dt) = 0;

		// Add an entity to the system
		virtual void	AddEntity(EntityId entity)
		{
			m_entities.emplace(entity);
		}

		// Remove an entity from the system
		virtual void	RemoveEntity(EntityId entity)
		{
			m_entities.erase(entity);
		}

		// Check if an entity is in the system
		virtual bool	HasEntity(EntityId entity) const
		{
			return m_entities.find(entity) != m_entities.end();
		}


	protected:
		std::unordered_set<EntityId>	m_entities;
	};
}

#endif