#include "GameObject.h"

#include "../Renderer/DirectX/D3DConstantBuffer.h"

namespace BINDU
{
	GameObject::GameObject()
	{
		XMStoreFloat4x4(&m_objectConstants.WorldMatrix,XMMatrixIdentity());
	}

	GameObject::GameObject(GameObject&& obj) noexcept
	{
		std::swap(this->m_transformComponent, obj.m_transformComponent);
		std::swap(this->m_constantBuffer, obj.m_constantBuffer);
		std::swap(this->m_rootParamSlot, obj.m_rootParamSlot);
		std::swap(this->m_objectConstants, obj.m_objectConstants);
		std::swap(this->m_cbIndex, obj.m_cbIndex);
	}

	GameObject& GameObject::operator=(GameObject&& obj) noexcept
	{
		std::swap(this->m_transformComponent, obj.m_transformComponent);
		std::swap(this->m_constantBuffer, obj.m_constantBuffer);
		std::swap(this->m_rootParamSlot, obj.m_rootParamSlot);
		std::swap(this->m_objectConstants, obj.m_objectConstants);
		std::swap(this->m_cbIndex, obj.m_cbIndex);

		return *this;
	}

	GameObject::~GameObject()
	{
		Close();
	}

	void GameObject::Initialize(const std::shared_ptr<D3DConstantBuffer>& constantBuffer)
	{
		m_constantBuffer = constantBuffer;
		m_cbIndex = m_constantBuffer->Allocate();
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

	void GameObject::Close()
	{
		if (m_constantBuffer)
		{
			m_constantBuffer->Free(m_cbIndex);
			m_constantBuffer.reset();
		}
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

			auto worldMat = rotationMat * scaleMat * translationMat;


			XMStoreFloat4x4(&m_objectConstants.WorldMatrix, XMMatrixTranspose(worldMat));

			// Update the entry in constant buffer
			m_constantBuffer->UpdateBuffer(m_cbIndex, m_objectConstants);

			m_transformComponent.SetDirty(false);
		}
	}
}
