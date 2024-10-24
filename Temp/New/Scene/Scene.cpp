
#include "Scene.h"

#include "../GameObject/MeshObject.h"
#include "../Renderer/DirectX/D3DCommandContext.h"

namespace BINDU
{
	Scene::Scene()
	{
	}

	void Scene::Update(double dt, const FrameResource* currFrame)
	{
		for (auto& layer : m_layerPool)
		{
			if (layer.second->IsUpdateActive())
			{
				layer.second->Update(dt, currFrame);
			}
		}
	}

	void Scene::Render(D3DCommandContext& commandContext, const FrameResource* currFrame)
	{
		for(auto& layer : m_layerPool)
		{
			if (layer.second->IsRenderActive())
			{
				layer.second->Render(commandContext, currFrame);
			}
		}
	}

/*	MeshObject* Scene::AddMeshObject(const std::shared_ptr<MeshObject>& gameObject)
	{
		m_gameObjectManager.AddGameObject(gameObject);

		auto light = dynamic_cast<Light*>(gameObject.get());

		if(light != nullptr)
		{
			m_lights.emplace_back(light);
		}

		return gameObject.get();
	}


	void Scene::RemoveMeshObject(MeshObject* gameObject)
	{
		m_gameObjectManager.RemoveGameObject(gameObject);
	} */

	RenderLayer* Scene::CreateLayer(const std::string& name, std::uint16_t index)
	{
		auto layer = std::make_unique<RenderLayer>();
		layer->SetName(name);

		if (index == UINT16_MAX)
			index = m_layerPool.size();

		layer->SetIndex(index);

		auto itr = m_layerPool.emplace(index, std::move(layer));

		return itr.first->second.get();
	}

	RenderLayer* Scene::GetLayer(std::uint16_t index)
	{
		return m_layerPool[index].get();
	}

	RenderLayer* Scene::AddLayer(std::unique_ptr<RenderLayer>&& renderLayer)
	{
		auto index = renderLayer->GetIndex();

		m_layerPool.emplace(index, std::move(renderLayer));

		return m_layerPool[index].get();
	}
	

	void Scene::RemoveLayer(const std::string& name)
	{
		std::uint16_t index = 0;

		for(auto& l : m_layerPool)
		{
			if(l.second->GetName() == name)
			{
				index = l.second->GetIndex();
			}
		}

		RemoveLayer(index);
	}

	void Scene::RemoveLayer(std::uint16_t index)
	{
		m_layerPool.erase(index);
	}

}
