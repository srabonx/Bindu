#ifndef BINDU_SCENE_H
#define BINDU_SCENE_H

#include "../ECS/EntityManager.h"
#include "../ECS/ComponentRegistry.h"
#include "../Renderer/CommandList.h"

namespace BINDU
{
	class ICameraController;
}


namespace BINDU
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		virtual ~Scene() = default;

		Entity	CreateEntity(const std::string& name = std::string());

		void	DestroyEntity(const Entity* entity);

		void	DestroyEntity(EntityId entity);

		virtual void	OnUpdate(double dt, ICameraController* cameraController) = 0;

		virtual void	OnRender(CommandList& commandList) = 0;

		Entity	CloneEntity(EntityId entityId);

		Entity	GetEntity(EntityId entityId);

		template <typename Component>
		const std::vector<EntityId>& GetEntitiesWithComponent() const
		{
			auto view = m_componentRegistry.GetView<Component>();

			return view.GetEntities();
		}

		template <typename... Components>
		const std::vector<EntityId>& GetEntitiesWithComponents() const
		{
			auto view = m_componentRegistry.GetView<Components>();

			return view.GetEntities();
		}

	protected:
		EntityManager		m_entityManager;

		ComponentRegistry	m_componentRegistry;

		std::map<std::uint32_t, std::unordered_set<EntityId>>	m_layerBatch;
	};
}

#endif