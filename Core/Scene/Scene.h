#ifndef SCENE_H
#define SCENE_H
#include "RenderLayer.h"
#include "../Managers/GameObjectManager.h"
#include "../Renderer/DirectX/FlyFrame.h"

namespace BINDU
{
	class Scene
	{
	public:

		explicit Scene(std::uint64_t maxObjects);
		~Scene() = default;

		void				Initialize();

		void				SetLightConstantRootParamIndex(std::uint16_t index);

		void				SetAmbientLight(const XMFLOAT4& ambientLight);

		void				Update(double dt, const FrameResource* currFrameResource);

		void				Render(const D3DCommandContext& commandContext, const FrameResource* currFrameResource);

		MeshObject*			AddMeshObject(const std::shared_ptr<MeshObject>& gameObject);

		void				RemoveMeshObject(MeshObject* gameObject);

		RenderLayer*		CreateLayer(const std::string& name, std::uint16_t index = UINT16_MAX);

		RenderLayer*		GetLayer(std::uint16_t index);

//		void				AddLayer(RenderLayer&& renderLayer);

		void				RemoveLayer(const std::string& name);

		void				RemoveLayer(std::uint16_t index);

	private:
		LightConstants		BindLights(MeshObject* gameObject) const;


	private:
		
		GameObjectManager						m_gameObjectManager;

		std::vector<Light*>						m_lights;

		std::map<std::uint16_t, RenderLayer>	m_layerPool;

		std::uint64_t							m_maxObjects;

		std::uint16_t							m_lightConstantRootParamIndex{ 0 };

		XMFLOAT4								m_ambientLight;

	};
}

#endif