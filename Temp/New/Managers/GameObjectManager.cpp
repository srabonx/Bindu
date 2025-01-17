#include "GameObjectManager.h"

#include "../GameObject/MeshObject.h"
#include "../GameObject/Light.h"

namespace BINDU
{
	GameObjectManager::GameObjectManager(std::uint64_t maxObjects) : m_maxObjects(maxObjects)
	{
		m_cbAllocator = std::make_shared<VariableSizeMemoryAllocator>(maxObjects);
	}

	void GameObjectManager::AddGameObject(std::shared_ptr<GameObject> gameObject)
	{

		m_gameObjectPool.emplace_back(gameObject);

		auto meshObject = dynamic_cast<MeshObject*>(gameObject.get());

		if (meshObject != nullptr)
		{
			CbAllocation allocation;

			allocation.Index = m_cbAllocator->Allocate(1);
			allocation.ParentAllocator = m_cbAllocator;

			meshObject->SetCbAllocation(allocation);

			m_meshObjectPool.emplace_back(meshObject);

			if (auto lightObject = dynamic_cast<Light*>(meshObject))
			{
				m_lightObjectPool.emplace_back(lightObject);
			}

		}
	}

	void GameObjectManager::RemoveGameObject(GameObject* gameObject)
	{
		auto guid = gameObject->GetGuid();


		if(auto meshObject = dynamic_cast<MeshObject*>(gameObject))
		{
			m_meshObjectPool.erase(std::remove_if(m_meshObjectPool.begin(), m_meshObjectPool.end(),
				[meshObject](MeshObject* meshObj) {return meshObject == meshObj; }));

			if (auto lightObject = dynamic_cast<Light*>(meshObject))
			{
				m_lightObjectPool.erase(std::remove_if(m_lightObjectPool.begin(), m_lightObjectPool.end(),
					[lightObject](Light* light)-> bool {return lightObject == light; }));
			}
		}

		m_gameObjectPool.erase(std::remove_if(m_gameObjectPool.begin(), m_gameObjectPool.end(),
			[guid](const std::shared_ptr<GameObject>& obj) {return obj->GetGuid() == guid; }));

		gameObject = nullptr;
	}

/*	void GameObjectManager::Update() const
	{
		for(auto& o : m_gameObjectPool)
		{
			o->Update();
		}
	}

	void GameObjectManager::Render(const D3DCommandContext& commandContext, const UploadBuffer* constantBuffer) const
	{
		for (auto& o : m_meshObjectPool)
		{
			o->Render(commandContext, constantBuffer);
		}
	}
	*/

	std::vector<MeshObject*>& GameObjectManager::GetMeshObjects()
	{
		return m_meshObjectPool;
	}

	std::vector<Light*>& GameObjectManager::GetLightObjects()
	{
		return m_lightObjectPool;
	}

	std::vector<std::shared_ptr<GameObject>>& GameObjectManager::GetGameObjects()
	{
		return m_gameObjectPool;
	}
}
