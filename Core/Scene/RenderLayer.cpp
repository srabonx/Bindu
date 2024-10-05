#include "RenderLayer.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/FlyFrame.h"
#include "../Managers/GameObjectManager.h"

namespace BINDU
{
	void RenderLayer::Initialize(const std::shared_ptr<GameObjectManager>& gameObjectManager)
	{
		m_parentObjectManager = gameObjectManager;
		m_initialized = true;
	}

	void RenderLayer::SetLightConstantRootParamIndex(std::uint16_t index)
	{
		m_lightConstantRootParamIndex = index;
	}

	void RenderLayer::AddMeshObject(std::shared_ptr<MeshObject> meshObject)
	{
		m_parentObjectManager.lock()->AddGameObject(meshObject);

		m_meshObjectPool.emplace_back(meshObject);
	}

	void RenderLayer::RemoveMeshObject(MeshObject* meshObject, bool removeFromParentObjectManager)
	{
		m_meshObjectPool.erase(
			std::remove_if(m_meshObjectPool.begin(), m_meshObjectPool.end(), [meshObject](const std::weak_ptr<MeshObject>& obj)->bool
				{
					return meshObject->GetGuid() == obj.lock()->GetGuid();
				}));

		if (removeFromParentObjectManager)
			m_parentObjectManager.lock()->RemoveGameObject(meshObject);

	}


	std::vector<std::weak_ptr<MeshObject>>& RenderLayer::GetMeshObjects()
	{
		return m_meshObjectPool;
	}

	void RenderLayer::Update(double dt, const FrameResource* currFrame)
	{
		auto matCb = currFrame->MaterialCb.get();

		auto lightCb = currFrame->LightCb.get();

		auto itr = m_meshObjectPool.begin();

		auto& lights = m_parentObjectManager.lock()->GetLightObjects();

		while (itr != m_meshObjectPool.end())
		{
			if (auto mesh = itr->lock())
			{
				mesh->Update();

				auto mat = mesh->GetMaterialComponent();

				// Update Material

				if (mat->IsDirty())
				{
					auto matConstant = mesh->GetMaterialComponent()->getMaterialConstants();

					matCb->CopyData(mat->GetCbAllocation()->Index, &matConstant);
				}

				// Bind lights
				if (lights.size() > 0)
				{
					auto lightConstants = BindLights(mesh.get(), lights);
					lightConstants.AmbientLight = m_ambientLight;
					
					lightCb->CopyData(mesh->GetCBAllocation()->Index, &lightConstants);
				}

			}
			else
			{
				itr = m_meshObjectPool.erase(itr);
				continue;
			}

			++itr;
		}

	}

	void RenderLayer::Render(const D3DCommandContext& commandContext, const FrameResource* currFrame)
	{
		if (m_pipelineState || m_rootSignature)
		{
			commandContext.BindPipeline(m_pipelineState.Get(), m_rootSignature.Get());
		}

		auto cmdList = commandContext.GetCommandList();

		auto matCb = currFrame->MaterialCb.get();

		auto lightCb = currFrame->LightCb.get();

		auto itr = m_meshObjectPool.begin();

		while(itr != m_meshObjectPool.end())
		{
			if (itr->expired()) { itr = m_meshObjectPool.erase(itr); continue; }

			auto obj = itr->lock();

			auto mat = obj->GetMaterialComponent();
			cmdList->SetGraphicsRootConstantBufferView(mat->GetConstantRootParameterIndex(),
				matCb->GetGPUVirtualAddressAt(mat->GetCbAllocation()->Index));

			cmdList->SetGraphicsRootConstantBufferView(m_lightConstantRootParamIndex,
				lightCb->GetGPUVirtualAddressAt(obj->GetCBAllocation()->Index));

			obj->Render(commandContext, currFrame->ObjectsCb.get());

			++itr;
		}

	}

	void RenderLayer::SetName(const std::string& name)
	{
		m_name = name;
	}

	std::string RenderLayer::GetName() const
	{
		return m_name;
	}

	void RenderLayer::SetIndex(int index)
	{
		m_index = index;
	}

	void RenderLayer::SetAmbientLight(const XMFLOAT4& ambientLight)
	{
		m_ambientLight = ambientLight;
	}

	int RenderLayer::GetIndex() const
	{
		return m_index;
	}

	void RenderLayer::SetPSO(ID3D12PipelineState* pso)
	{
		m_pipelineState = pso;
	}

	ID3D12PipelineState* RenderLayer::GetPSO() const
	{
		return m_pipelineState.Get();
	}

	void RenderLayer::SetRootSig(ID3D12RootSignature* rootSig)
	{
		m_rootSignature = rootSig;
	}

	ID3D12RootSignature* RenderLayer::GetRootSig() const
	{
		return m_rootSignature.Get();
	}

	void RenderLayer::SetRenderActive(bool active)
	{
		m_isRenderActive = active;
	}

	bool RenderLayer::IsRenderActive() const
	{
		return m_isRenderActive;
	}

	void RenderLayer::SetUpdateActive(bool active)
	{
		m_isUpdateActive = active;
	}

	bool RenderLayer::IsUpdateActive() const
	{
		return m_isUpdateActive;
	}

	std::weak_ptr<GameObjectManager>& RenderLayer::GetParentObjectManager()
	{
		return m_parentObjectManager;
	}

	LightConstants RenderLayer::BindLights(MeshObject* gameObject, const std::vector<Light*>& lights) const
	{
		const int MAX_LIGHTS = 16;

		std::vector<Light*> relevantLights;
		relevantLights.reserve(MAX_LIGHTS);

		LightConstants lc = {};

		auto objPos = gameObject->GetTransformComponent()->GetTranslation3f();

		for (auto& light : lights)
		{
			if (light->IsWithinRange(objPos))
			{
				relevantLights.emplace_back(light);
			}
			else
			{
				continue;
			}

			auto lightType = light->GetLightType();

			switch (lightType)
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


		for (int i = 0; i < relevantLights.size(); i++)
		{
			Light* light = relevantLights[i];

			lc.LightData[i] = light->GetLightData();
		}

		return lc;
	}

}
