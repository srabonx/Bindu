
#include "Scene.h"

#include "../GameObject/MeshObject.h"
#include "../Renderer/DirectX/D3DCommandContext.h"

namespace BINDU
{
	Scene::Scene(std::uint64_t maxObjects) : m_gameObjectManager(maxObjects) , m_maxObjects(maxObjects)
	{
	}

	void Scene::Initialize()
	{
		
	}

	void Scene::SetLightConstantRootParamIndex(std::uint16_t index)
	{
		m_lightConstantRootParamIndex = index;
	}

	void Scene::SetAmbientLight(const XMFLOAT4& ambientLight)
	{
		m_ambientLight = ambientLight;
	}

	void Scene::Update(double dt, const FrameResource* currFrameResource)
	{


		auto meshObjects = m_gameObjectManager.GetMeshObjects();

		auto matCb = currFrameResource->MaterialCb.get();

		auto lightCb = currFrameResource->LightCb.get();

		for (auto& m : meshObjects)
		{
			m->Update();

			auto mat = m->GetMaterialComponent();

			if (mat->IsDirty())
			{
				MaterialConstants materialConstants = mat->getMaterialConstants();

			/*	materialConstants.DiffuseAlbedo = mat->GetDiffuseAlbedo4f();
				materialConstants.FresnelR0 = mat->GetFresnelR03f();
				materialConstants.Roughness = mat->GetRoughness();
				materialConstants.MatTransform = mat->GetTransform4x4(); */

				matCb->CopyData(mat->GetCbAllocation()->Index, &materialConstants);
			}

			if (m_lights.size() > 0)
			{
				auto lightConstant = BindLights(m);

				lightConstant.AmbientLight = m_ambientLight;

				lightCb->CopyData(m->GetCBAllocation()->Index, &lightConstant);
			}
		}
	}

	void Scene::Render(const D3DCommandContext& commandContext, const FrameResource* currFrameResource)
	{
		for(auto& layer : m_layerPool)
		{
			if (layer.second.IsActive())
			{
				layer.second.Render(commandContext, currFrameResource);
			}
		}
	}

	MeshObject* Scene::AddMeshObject(const std::shared_ptr<MeshObject>& gameObject)
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
	}

	RenderLayer* Scene::CreateLayer(const std::string& name, std::uint16_t index)
	{
		RenderLayer layer;
		layer.SetName(name);

		if (index == UINT16_MAX)
			index = m_layerPool.size();

		layer.SetLightConstantRootParamIndex(m_lightConstantRootParamIndex);
		layer.SetIndex(index);

		auto itr = m_layerPool.emplace(index, std::move(layer));

		return &itr.first->second;
	}

	RenderLayer* Scene::GetLayer(std::uint16_t index)
	{
		return &m_layerPool[index];
	}

	/*	void Scene::AddLayer(RenderLayer&& renderLayer)
	{
		m_layerPool.emplace(renderLayer.GetIndex(), std::move(renderLayer));
	}
	*/

	void Scene::RemoveLayer(const std::string& name)
	{
		std::uint16_t index = 0;

		for(auto& l : m_layerPool)
		{
			if(l.second.GetName() == name)
			{
				index = l.second.GetIndex();
			}
		}

		RemoveLayer(index);
	}

	void Scene::RemoveLayer(std::uint16_t index)
	{
		m_layerPool.erase(index);
	}

	LightConstants Scene::BindLights(MeshObject* gameObject) const
	{
		const int MAX_LIGHTS = 16;

		std::vector<Light*> relevantLights;

		LightConstants lc = {};

		auto objPos = gameObject->GetTransformComponent()->GetTranslation3f();

		for(auto& light : m_lights)
		{
			if (light->IsWithinRange(objPos))
			{
				relevantLights.push_back(light);
			}

			auto lightType = light->GetLightType();

			switch(lightType)
			{
			case LightType::DIRECTIONAL:
				lc.NumOfDirLight++;
				break;
			case LightType::POINT:
				lc.NumOfPointLight++;
				break;
			case LightType::SPOT:
				lc.NumOfSpotLight++;
				break;
			}

			if (relevantLights.size() >= MAX_LIGHTS)
				break;
		}


		for(int i = 0; i < relevantLights.size(); i++)
		{
			Light* light = relevantLights[i];

			lc.LightData[i] = light->GetLightData();
		}

		return lc;
	}
}
