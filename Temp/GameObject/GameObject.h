#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderable.h"

namespace BINDU
{
	class GameObject : public Renderable
	{
	public:
		GameObject(std::uint32_t indexCount, std::uint32_t startIndexLocation, int baseVertexLocation, FlyFrame* parentFrame);
		virtual			~GameObject() {}

	protected:
		


	};
}

#endif