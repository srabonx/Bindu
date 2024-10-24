#ifndef BINDU_SCENE_LAYER_H
#define BINDU_SCENE_LAYER_H

#include <vector>
#include "../ECS/ECSConstants.h"

#include "../Utility/Common/CommonUtility.h"
#include "../ECS/Entity.h"

namespace BINDU
{
	class Scene;
	class Shader;

	class SceneLayer
	{
	public:
		SceneLayer(Scene* parent) {};
		~SceneLayer() = default;



	private:
		Scene*					m_parentScene{ nullptr };

		Ref<Shader>				m_sceneShader{ nullptr };

		std::vector<EntityId>	m_entities;

		std::uint32_t			m_layerId{ 0 };
	};
}

#endif