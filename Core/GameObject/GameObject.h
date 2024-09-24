#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <DirectXMath.h>
#include "Transform.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"



namespace BINDU
{
	class D3DDeviceManager;
	class D3DCommandContext;
	class D3DPipelineStateManager;
	class D3DConstantBuffer;

	using namespace DirectX;

	struct ObjectConstants
	{
		XMFLOAT4X4		WorldMatrix;
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

		virtual void	 Initialize(const std::shared_ptr<D3DConstantBuffer>& constantBuffer);

		virtual void	 Update() = 0;

		virtual void	 Render(const D3DCommandContext& commandContext) = 0;


		void			 SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot);


		Transform*		 GetTransformComponent();

		void			 SetTransform(const Transform& transform);

		void			 Close();

	protected:

		void			 UpdateConstantBuffer();

	protected:

		ObjectConstants						m_objectConstants;

		std::shared_ptr<D3DConstantBuffer>	m_constantBuffer{ nullptr };

	//	std::shared_ptr<UploadBuffer>		m_constantBuffer;

		Transform							m_transformComponent;

		std::uint8_t						m_rootParamSlot{0};

		std::uint64_t						m_cbIndex{ 0 };

	};

}

#endif