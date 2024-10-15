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
		if (name.empty())
			tagCompo.Tag = "Entity " + std::to_string(entityId);
		else
			tagCompo.Tag = name;

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
		auto view = m_componentRegistry.GetView<TagComponent, TransformComponent>();

		for (auto itr = view.Begin(); itr != view.End(); ++itr)
		{
			auto tagCompo = view.GetComponent<TagComponent>((*itr));

			OutputDebugStringA(tagCompo.Tag.c_str());
			OutputDebugStringA("\n");
		}

		for (auto itr = view.Begin(); itr != view.End(); ++itr)
		{
			auto [tag] = view.Get<TagComponent>((*itr));

			if (!tag.Tag.empty())
			{
				OutputDebugStringA(tag.Tag.c_str());
				OutputDebugStringA("\n");
			}
		}

		for(auto itr = view.Begin(); itr != view.End(); ++itr)
		{
			auto [tag, transform] = view.Get<TagComponent, TransformComponent>((*itr));

			if (!tag.Tag.empty())
			{
				OutputDebugStringA(tag.Tag.c_str());
				OutputDebugStringA("\n");
			}
		}
	}
}
