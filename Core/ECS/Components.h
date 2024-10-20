#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <iostream>
#include <DirectXMath.h>
#include "../../MathHelper/MathHelper.h"
#include "../GameObject/MeshManager.h"
#include "../Resources/Texture.h"

namespace BINDU
{

	using namespace DirectX;

	struct TagComponent
	{
	public:
		std::string Tag = "Default";


		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(std::string tag) : Tag(std::move(tag)) {}
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

	struct MeshComponent
	{
		MeshData	Mesh;

		MeshComponent() = default;
		MeshComponent(MeshData mesh) : Mesh(std::move(mesh)) {}
	};

	struct MaterialComponent
	{
		XMFLOAT4			DiffuseAlbedo{ 1.0f,1.0f,1.0f,1.0f };

		XMFLOAT3			FresnelR0{ 0.01f,0.01f,0.01f };

		float				Roughness{ 0.25f };

		XMFLOAT4X4			MatTransform = MathHelper::Identity4x4();

		MaterialComponent() = default;
		MaterialComponent(const XMFLOAT4& diffuseAlbedo, const XMFLOAT3& fresnel, float roughness, const XMFLOAT4X4& transform) :
			DiffuseAlbedo(diffuseAlbedo), FresnelR0(fresnel), Roughness(roughness), MatTransform(transform) {}
	};

	struct TextureComponent
	{
		std::shared_ptr<Texture>	Tex;

		TextureComponent() = default;
		TextureComponent(const std::shared_ptr<Texture>& texture) : Tex(texture) {}
	};

}

#endif