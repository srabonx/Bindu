#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <iostream>
#include <DirectXMath.h>
#include "../MathHelper/MathHelper.h"
#include "../Geometry/MeshData.h"
#include "../Camera/EditorCamera.h"

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

		TransformComponent(const TransformComponent&) = default;
		TransformComponent& operator = (const TransformComponent&) = default;

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
		MeshComponent(MeshData data) : Data(std::move(data)) {}
		MeshComponent(const MeshComponent&) = default;
		MeshComponent& operator = (const MeshComponent&) = default;

	};

	struct MaterialComponent
	{
		XMFLOAT4	DiffuseAlbedo{ 1.0f, 1.0f, 1.0f, 1.0f };
		XMFLOAT3	FresnelR0{ 0.01f, 0.01f, 0.01f };
		float		Roughness{ 0.25f };
		XMFLOAT4X4	MatTransform = MathHelper::Identity4x4();

		XMFLOAT3	EmissiveColor{ 0.0f, 0.0f, 0.0f }; // Default to black (no emissive)
		float		EmissiveIntensity{ 0.0f }; // Default to 0, meaning no emission

		bool		IsDirty{ true };

		MaterialComponent() = default;

		MaterialComponent(
			const XMFLOAT4& diffuseAlbedo,
			const XMFLOAT3& fresnel,
			float roughness,
			const XMFLOAT4X4& transform,
			const XMFLOAT3& emissiveColor = { 0.0f, 0.0f, 0.0f },
			float emissiveIntensity = 0.0f
		) :
			DiffuseAlbedo(diffuseAlbedo),
			FresnelR0(fresnel),
			Roughness(roughness),
			MatTransform(transform),
			EmissiveColor(emissiveColor),
			EmissiveIntensity(emissiveIntensity) {}

		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent& operator=(const MaterialComponent&) = default;
	};

	struct LightComponent
	{
		enum class LightType : std::uint8_t { DIRECTIONAL, POINT, SPOT };

		LightType		Type;
		XMFLOAT4		Color{ 0.0f,0.0f,0.0f,1.0f };
		float			Intensity{ 0.0f };
		XMFLOAT3		Position{0.0f,0.0f,0.0f};
		XMFLOAT3		Direction{ 0.0f,0.0f,0.0f };
		float			Range{ 10.0f };
		float			Attenuation{ 10.f };
	};

	struct TextureComponent
	{

	};

	struct LayerComponent
	{
		std::uint32_t	LayerIndex{ 0 };

		LayerComponent() = default;
		LayerComponent(std::uint32_t index) : LayerIndex(index) {}
		LayerComponent(const LayerComponent&) = default;
		LayerComponent& operator = (const LayerComponent&) = default;
	};

	struct ShaderComponent
	{
		WeakRef<Shader> ShaderData;

		ShaderComponent() = default;
		ShaderComponent(const Ref<Shader>& shader) : ShaderData(shader) {}
		ShaderComponent(const ShaderComponent&) = default;
		ShaderComponent& operator = (const ShaderComponent&) = default;
	};

	struct CameraComponent
	{
		WeakRef<Camera> Camera;

		bool			Primary = true;

		EntityId		Target{ InvalidIndex };

		float			Distance{ -5.f };

		float			HeightOffset{ 4.f };

		float			SmoothingFactor{ 0.5f };

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent& operator = (const CameraComponent&) = default;
	};

}

#endif