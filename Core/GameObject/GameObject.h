#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <DirectXMath.h>
#include "Transform.h"
#include "../Renderer/DirectX/UploadBuffer.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"



namespace BINDU
{
	class D3DDeviceManager;
	class D3DCommandContext;
	class D3DPipelineStateManager;

	using namespace DirectX;

	struct ObjectConstant
	{
		XMFLOAT4X4		WorldMatrix;
	};

	class GameObject
	{
	public:
		GameObject();

		GameObject(const GameObject& obj);

		GameObject& operator = (const GameObject& obj);

		virtual ~GameObject() {}

		virtual void	 Initialize(const D3DDeviceManager& deviceManager);

		virtual void	 Update() = 0;

		virtual void	 Render(const D3DCommandContext& commandContext) = 0;


		void			 SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot);


		Transform*		 GetTransformComponent();

		void			 SetTransform(const Transform& transform);

	protected:

		void			 UpdateConstantBuffer();

	protected:

		ObjectConstant						m_objectConstants;

		std::shared_ptr<UploadBuffer>		m_constantBuffer;

		Transform							m_transformComponent;

		std::uint8_t						m_rootParamSlot{0};

	};

}

#endif