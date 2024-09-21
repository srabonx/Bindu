#include "GameObject.h"

#include "../Renderer/DirectX/CpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"

namespace BINDU
{
	GameObject::GameObject()
	{
		m_constantBuffer = std::make_shared<UploadBuffer>(true);

		XMStoreFloat4x4(&m_objectConstants.WorldMatrix,XMMatrixIdentity());
	}

	GameObject::GameObject(const GameObject& obj)
	{
		this->m_transformComponent = obj.m_transformComponent;
		this->m_constantBuffer = obj.m_constantBuffer;
		this->m_rootParamSlot = obj.m_rootParamSlot;
		this->m_objectConstants = obj.m_objectConstants;
	}

	GameObject& GameObject::operator=(const GameObject& obj)
	{
		if (this == &obj)
			return *this;

		this->m_transformComponent = obj.m_transformComponent;
		this->m_constantBuffer = obj.m_constantBuffer;
		this->m_rootParamSlot = obj.m_rootParamSlot;
		this->m_objectConstants = obj.m_objectConstants;
		return *this;
	}

	void GameObject::Initialize(const D3DDeviceManager& deviceManager)
	{
		auto d3dDevice = deviceManager.GetD3DDevice();
		m_constantBuffer->Initialize<ObjectConstant>(d3dDevice, 1);
	}

	void GameObject::SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot)
	{
		m_rootParamSlot = RootParamSlot;
	}

	Transform* GameObject::GetTransformComponent()
	{
		return &m_transformComponent;
	}

	void GameObject::SetTransform(const Transform& transform)
	{
		m_transformComponent = transform;
	}

	void GameObject::UpdateConstantBuffer()
	{
		if (m_transformComponent.IsDirty())
		{
			auto rotationVec = m_transformComponent.GetRotation();
			auto scaleVec = m_transformComponent.GetScale();
			auto translationVec = m_transformComponent.GetTranslation();


			auto rotationMat = XMMatrixRotationRollPitchYawFromVector(rotationVec);
			auto scaleMat = XMMatrixScalingFromVector(scaleVec);
			auto translationMat = XMMatrixTranslationFromVector(translationVec);

			auto worldMat = translationMat * rotationMat * scaleMat;


			XMStoreFloat4x4(&m_objectConstants.WorldMatrix, XMMatrixTranspose(worldMat));


			m_constantBuffer->CopyData(0, m_objectConstants);

			m_transformComponent.SetDirty(false);
		}
	}
}
