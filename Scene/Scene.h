#ifndef BINDU_SCENE_H
#define BINDU_SCENE_H

#include "../ECS/EntityManager.h"
#include "../ECS/ComponentRegistry.h"
#include "../MathHelper/MathHelper.h"
#include "../Renderer/CommandList.h"

namespace BINDU
{
	class Camera;
}


namespace BINDU
{
	class Entity;

	struct SceneData
	{
		XMFLOAT4X4		ViewProjMatrix = MathHelper::Identity4x4();
	};

	struct EntityData
	{
		XMFLOAT4X4		WorldMatrix = MathHelper::Identity4x4();
	};

	class Scene
	{
		friend class Entity;

	public:
		Scene(std::uint32_t entityCount);
		~Scene() = default;

		Entity	CreateEntity(const std::string& name = std::string());

		void	DestroyEntity(const Entity* entity);

		void	DestroyEntity(EntityId entity);

		void	OnUpdate(double dt);

		void	OnRender(CommandList& commandList);

		Entity	GetEntity(EntityId entityId);

	private:
		EntityManager		m_entityManager;

		ComponentRegistry	m_componentRegistry;

		std::map<std::uint32_t, std::unordered_set<EntityId>>	m_layerBatch;

		Ref<UniformBuffer>	m_entityDataBuffer{ nullptr };

		Ref<UniformBuffer>	m_sceneDataBuffer{ nullptr };

		SceneData			m_sceneData;

		Camera*				m_currentPrimaryCamera{ nullptr };
	};
}

#endif