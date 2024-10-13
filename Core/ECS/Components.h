#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "IComponent.h"
#include <string>
#include <iostream>
#include <DirectXMath.h>

namespace BINDU
{

	using namespace DirectX;

	struct NameComponent : public IComponent
	{
	public:
		NameComponent() = default;
		std::string Name = "Default";
	};

	// Transform component
	struct TransformComponent : public IComponent
	{
	public:
		XMFLOAT3	Position;
		XMFLOAT3	Rotation;
		XMFLOAT3	Scale;

		TransformComponent()
			: Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f) {}
	};

}

#endif