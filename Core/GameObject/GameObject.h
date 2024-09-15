#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transformable.h"
#include "Renderable.h"

namespace BINDU
{
	class GameObject : public Transformable, public Renderable
	{
	public:
		virtual			~GameObject() {}

	protected:
		


	};
}

#endif