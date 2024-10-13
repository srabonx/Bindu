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

		explicit Scene();
		virtual ~Scene() {};

		virtual void		Update(double dt, const FrameResource* currFrame);

		virtual void		Render(D3DCommandContext& commandContext, const FrameResource* currFrame);

/*		MeshObject* AddMeshObject(const std::shared_ptr<MeshObject>& gameObject);

		void				RemoveMeshObject(MeshObject* gameObject); */

		RenderLayer*		CreateLayer(const std::string& name, std::uint16_t index = UINT16_MAX);

		RenderLayer*		GetLayer(std::uint16_t index);

		RenderLayer*		AddLayer(std::unique_ptr<RenderLayer>&& renderLayer);

		void				RemoveLayer(const std::string& name);

		void				RemoveLayer(std::uint16_t index);


	private:
		std::string								m_name;

//		std::vector<Light*>						m_lights;

		std::map<std::uint16_t, std::unique_ptr<RenderLayer>>	m_layerPool;
	};
}

#endif