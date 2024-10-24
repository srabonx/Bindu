#include "Scene.h"

#include "../ECS/Components.h"
#include "../ECS//Entity.h"
#include "../Debug/Assert.h"
#include "../Debug/Profiler.h"
#include "../Renderer/ResourceFactory.h"
#include "../Renderer/Shader.h"

namespace BINDU
{
	Scene::Scene(std::uint32_t entityCount)
	{
		m_entityDataBuffer = ResourceFactory::CreateUniformBuffer(entityCount, sizeof(EntityData));

		m_sceneDataBuffer = ResourceFactory::CreateUniformBuffer(1, sizeof(SceneData));
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entityId = m_entityManager.CreateEntity();

		Entity entity = { entityId, this };

		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<LayerComponent>();

		m_layerBatch[0].insert(entityId);

		auto& tagCompo = entity.GetComponent<TagComponent>();

		tagCompo.Tag = name.empty() ? "Entity " + std::to_string(entityId) : name;

		return entity;
	}

	void Scene::DestroyEntity(const Entity* entity)
	{
		if (entity)
			DestroyEntity(entity->GetId());
	}

	void Scene::DestroyEntity(EntityId entity)
	{
		m_entityManager.RemoveEntity(entity);
		m_componentRegistry.RemoveEntity(entity);
	}

	void Scene::OnUpdate(double dt)
	{
		auto view = m_componentRegistry.GetView<TransformComponent>();

		for(const auto& e : view)
		{
			auto& transformCompo = m_componentRegistry.GetComponent<TransformComponent>(e);

			if (transformCompo.IsDirty)
			{
				EntityData data = { transformCompo.GetTransform4x4() };

				m_entityDataBuffer->SetData(&data, sizeof(EntityData), e);

				transformCompo.IsDirty = false;
			}
		}

		auto cameraView = m_componentRegistry.GetView<CameraComponent, TransformComponent>();

		XMMATRIX cameraTransform{};

		for(auto& cameraEntity : cameraView)
		{
			auto [camCompo, transformCompo] = view.Get<CameraComponent, TransformComponent>(cameraEntity);

			auto camera = camCompo.Camera.lock();

			if(camera)
			{
				if(camCompo.Primary)
				{
					m_currentPrimaryCamera = camera.get();
					cameraTransform = transformCompo.GetTransform();

					auto view = XMMatrixInverse(nullptr, cameraTransform);
					auto proj = camera->GetProjection();

					auto viewProj = view * proj;

					XMStoreFloat4x4(&m_sceneData.ViewProjMatrix, XMMatrixTranspose(viewProj));
				}
			}

		}

		m_sceneDataBuffer->SetData(&m_sceneData, sizeof(SceneData), 0);
	}

	void Scene::OnRender(CommandList& commandList)
	{
		BINDU_PROFILE_FUNCTION();


		for(auto& layer : m_layerBatch)
		{
			for(const auto& e : layer.second)
			{
				auto& layerComponent = m_componentRegistry.GetComponent<LayerComponent>(e);

				// If Layer Component is dirty
				if(layerComponent.LayerIndex != layer.first)
				{
					layer.second.erase(e);

					m_layerBatch[layerComponent.LayerIndex].emplace(e);
				}

				auto& shaderComponent = m_componentRegistry.GetComponent<ShaderComponent>(e);

				if(auto shader = shaderComponent.ShaderData.lock())
				{
					if (commandList.BindShader(shader.get()))
					{
						shader->BindUniformBuffer(shaderComponent.SceneDataUniformBufferSlot, e, m_sceneDataBuffer.get());
					}

					shader->BindUniformBuffer(shaderComponent.EntityDataUniformBufferSlot, e, m_entityDataBuffer.get());

					auto& meshComponent = m_componentRegistry.GetComponent<MeshComponent>(e);

					if (meshComponent.Data.p_IndexBuffer)
						commandList.DrawMesh(meshComponent.Data);
				}

			}
		}
	}

	Entity Scene::GetEntity(EntityId entityId)
	{
		BINDU_CORE_ASSERT(m_entityManager.HasEntity(entityId), "Entity does not exist!");

		return { entityId, this };
	}
}
