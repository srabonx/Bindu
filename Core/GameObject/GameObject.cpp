#include "GameObject.h"

#include "../Renderer/DirectX/CpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"

namespace BINDU
{
	GameObject::GameObject() : m_constantBuffer(true)
	{
		XMStoreFloat4x4(&m_worldMatrix,XMMatrixIdentity());
	}

	void GameObject::Initialize(const D3DDeviceManager& deviceManager)
	{
		auto d3dDevice = deviceManager.GetD3DDevice();
		m_constantBuffer.Initialize<ObjectConstant>(d3dDevice, 1);
	}

	void GameObject::Update()
	{
		auto rotationVec = m_transformComponent.GetRotation();
		auto scaleVec = m_transformComponent.GetScale();
		auto translationVec = m_transformComponent.GetTranslation();


		auto rotationMat	 = XMMatrixRotationRollPitchYawFromVector(rotationVec);
		auto scaleMat = XMMatrixScalingFromVector(scaleVec);
		auto translationMat = XMMatrixTranslationFromVector(translationVec);

		auto worldMat = translationMat * rotationMat * scaleMat;
		

		XMStoreFloat4x4(&m_worldMatrix, XMMatrixTranspose(worldMat));



		ObjectConstant oc;
		XMStoreFloat4x4(&oc.WorldMatrix, XMMatrixTranspose(worldMat));


		m_constantBuffer.CopyData(0,oc);
	}


	void GameObject::SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot)
	{
		m_rootParamSlot = RootParamSlot;
	}

	Transform* GameObject::GetTransformComponent()
	{
		return &m_transformComponent;
	}

}
