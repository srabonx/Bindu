#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <iostream>
#include <DirectXMath.h>
#include "../MathHelper/MathHelper.h"
#include "../Geometry/MeshData.h"
#include "../Renderer/EditorCamera.h"

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
		XMFLOAT3	Position{ 0.0f,0.0,0.0f };
		XMFLOAT3	Rotation{ 0.0f,0.0f,0.0f };
		XMFLOAT3	Scale{ 1.0f,1.0f,1.0f };

		bool		IsDirty{ true };

		TransformComponent() = default;
		TransformComponent(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) :
			Position(position), Rotation(rotation), Scale(scale), IsDirty(true) {}

		void		SetPosition(const XMFLOAT3& position)
		{
			Position = position;
			IsDirty = true;
		}

		void		SetRotation(const XMFLOAT3& rotation)
		{
			Rotation = rotation;
			IsDirty = true;
		}

		void		SetScale(const XMFLOAT3& scale)
		{
			Scale = scale;
			IsDirty = true;
		}

		XMFLOAT4X4 GetTransform4x4() const
		{
			auto rotationVec = XMLoadFloat3(&Rotation);
			auto scaleVec = XMLoadFloat3(&Scale);
			auto translationVec = XMLoadFloat3(&Position);

			auto rotationMat = XMMatrixRotationRollPitchYawFromVector(rotationVec);
			auto scaleMat = XMMatrixScalingFromVector(scaleVec);
			auto translationMat = XMMatrixTranslationFromVector(translationVec);


			auto transform = rotationMat * scaleMat * translationMat;

			XMFLOAT4X4 shaderTransform = {};

			XMStoreFloat4x4(&shaderTransform, XMMatrixTranspose(transform));

			return shaderTransform;
		}

		XMMATRIX GetTransform() const
		{
			auto rotationVec = XMLoadFloat3(&Rotation);
			auto scaleVec = XMLoadFloat3(&Scale);
			auto translationVec = XMLoadFloat3(&Position);

			auto rotationMat = XMMatrixRotationRollPitchYawFromVector(rotationVec);
			auto scaleMat = XMMatrixScalingFromVector(scaleVec);
			auto translationMat = XMMatrixTranslationFromVector(translationVec);


			return rotationMat * scaleMat * translationMat;
		}
	};

	struct MeshComponent
	{
	public:
		MeshData	Data;

		MeshComponent() = default;
		MeshComponent(const MeshData& data) : Data(data) {}
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

	};

	struct LayerComponent
	{
		std::uint32_t	LayerIndex{ 0 };

		LayerComponent() = default;
		LayerComponent(std::uint32_t index) : LayerIndex(index) {}
	};

	struct ShaderComponent
	{
		WeakRef<Shader> ShaderData;

		std::uint16_t	SceneDataUniformBufferSlot{ 0 };

		std::uint16_t	EntityDataUniformBufferSlot{ 0 };


		ShaderComponent() = default;
		ShaderComponent(const Ref<Shader>& shader) : ShaderData(shader) {}
	};

	struct CameraComponent
	{
		WeakRef<Camera> Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}

#endif