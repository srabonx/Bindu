#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <vector>
#include "../GameObject/GameObject.h"

namespace BINDU
{
	class MeshObject;
}

namespace BINDU
{
	class GameObjectManager
	{
	public:
		GameObjectManager(std::uint64_t maxObjects);

		~GameObjectManager() = default;

		void				AddGameObject(std::shared_ptr<GameObject> gameObject);

		void				RemoveGameObject(GameObject* gameObject);

		void				Update() const;

		void				Render(const D3DCommandContext& commandContext, const UploadBuffer* constantBuffer) const;

		std::vector<std::shared_ptr<GameObject>>& GetGameObjects();

		std::vector<MeshObject*>&				  GetMeshObjects();

	private:

		std::vector<std::shared_ptr<GameObject>>		m_gameObjectPool;

		std::vector<MeshObject*>						m_meshObjectPool;

		std::shared_ptr<VariableSizeMemoryAllocator>	m_cbAllocator{ nullptr };

		std::uint64_t									m_maxObjects{ 0 };
	};
}

#endif
