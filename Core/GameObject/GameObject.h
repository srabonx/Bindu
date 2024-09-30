#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <DirectXMath.h>

#include "LightData.h"
#include "Transform.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"
#include "../../MathHelper/MathHelper.h"



namespace BINDU
{
	class UploadBuffer;
	class D3DCommandContext;


	using namespace DirectX;

	struct ObjectConstants
	{
		XMFLOAT4X4			WorldMatrix = MathHelper::Identity4x4();

		XMFLOAT4X4			TextureTransform = MathHelper::Identity4x4();

	/*	XMFLOAT4			DiffuseAlbedo{ 1.0f,1.0f,1.0f,1.0f };

		XMFLOAT3			FresnelR0{ 0.01f,0.01f,0.01f };

		float				Roughness{ 0.25f };

		XMFLOAT4X4			MatTransform = MathHelper::Identity4x4();

		XMFLOAT4			AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

		LightData			LightData[16]; */
	};

	class GameObject
	{
	public:
		GameObject();

		GameObject(const GameObject& obj) = delete;

		GameObject& operator = (const GameObject& obj) = delete;

		GameObject(GameObject&& obj) noexcept;

		GameObject& operator = (GameObject&& obj) noexcept;

		virtual ~GameObject();

		virtual void	 Update() = 0;

		virtual void	 Render(const D3DCommandContext& commandContext, const UploadBuffer* constantBuffer) = 0;

		Transform*		 GetTransformComponent();

		void			 SetTransform(const Transform& transform);

		UUID			 GetGuid() const;


	protected:

		UUID								m_guid{ 0 };

		Transform							m_transformComponent;

	};

}

#endif