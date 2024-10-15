#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <iostream>
#include <DirectXMath.h>
#include "../../MathHelper/MathHelper.h"

namespace BINDU
{

	using namespace DirectX;

	struct TagComponent
	{
	public:
		std::string Tag = "Default";


		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	// Transform component
	struct TransformComponent
	{
	public:
		XMFLOAT4X4	Transform = MathHelper::Identity4x4();

		TransformComponent() = default;
		TransformComponent(const XMFLOAT4X4& transform) : Transform(transform) {}
		TransformComponent(const XMMATRIX& transform) { XMStoreFloat4x4(&Transform, XMMatrixTranspose(transform)); }
	};

}

#endif