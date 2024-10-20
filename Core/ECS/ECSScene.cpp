#include "ECSScene.h"

#include "Entity.h"
#include "Components.h"

namespace BINDU
{
	Entity ECSScene::CreateEntity(const std::string& name)
	{
		auto entityId = m_entityManager.CreateEntity();

		Entity entity = { entityId, this };

		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();

		auto& tagCompo = entity.GetComponent<TagComponent>();

		tagCompo.Tag = name.empty() ? "Entity " + std::to_string(entityId) : name;

		return entity;
	}

	void ECSScene::DestroyEntity(const Entity* entity)
	{
		if (entity)
			DestroyEntity(entity->GetId());
	}

	void ECSScene::DestroyEntity(EntityId entity)
	{
		m_entityManager.RemoveEntity(entity);
		m_componentRegistry.RemoveEntity(entity);
	}

	void ECSScene::OnUpdate(double dt)
	{
		
	}

	void ECSScene::OnRender(D3DCommandContext& commandContext, const FrameResource* currentFrame)
	{
		auto view = m_componentRegistry.GetView<MeshComponent>()
	}

	Entity ECSScene::GetEntity(EntityId entityId)
	{
		BINDU_ASSERT(m_entityManager.HasEntity(entityId), "Entity does not exist!");

		return { entityId, this };
	}
}
