#include "GameObject.h"

#include "../Renderer/DirectX/D3DCommandContext.h"

namespace BINDU
{
	GameObject::GameObject(std::uint32_t indexCount, std::uint32_t startIndexLocation, int baseVertexLocation,
		FlyFrame* parentFrame) : Renderable(indexCount, startIndexLocation, baseVertexLocation, parentFrame)
	{
	}
}
