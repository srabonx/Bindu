#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <cstdint>
#include <unordered_set>
#include <array>
#include "../Renderer/DirectX/VariableSizeMemoryAllocator.h"

#include "IComponent.h"

namespace BINDU
{

	using EntityId = std::uint32_t;
	using EntitySig = std::uint64_t;

	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		EntityId	CreateEntity();

		void		RemoveEntity(EntityId entity);				

		bool		HasEntity(EntityId entity);

	private:
		std::unordered_set<EntityId>						m_entities;
		VariableSizeMemoryAllocator							m_entityAllocator{ UINT32_MAX };

	};
}

#endif