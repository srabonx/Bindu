#include "Camera.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"
#include "../Renderer/DirectX/UploadBuffer.h"
#include "../Renderer/DirectX/CpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DPipelineStateManager.h"


namespace BINDU
{
	Camera::Camera() : m_cameraViewConstants(true)
	{
	}

	void Camera::Initialize(const D3DDeviceManager& deviceManager)
	{
		auto d3dDevice = deviceManager.GetD3DDevice();
		m_constantBuffer.Initialize<ObjectConstant>(d3dDevice, 1);

		m_cameraViewConstants.Initialize<CameraViewConstant>(d3dDevice,1);

	}

	void Camera::Update()
	{
		GameObject::Update();

		auto translation = m_transformComponent.GetTranslation();

		m_eyePos.x = m_radius * sinf(m_phi) * cosf(m_theta);
		m_eyePos.y = m_radius * cosf(m_phi);
		m_eyePos.z = m_radius * sinf(m_phi) * sinf (m_theta);

		XMVECTOR pos = XMVectorSet(m_eyePos.x, m_eyePos.y, m_eyePos.z,1.0f);

		XMVECTOR target = XMVectorZero();

		XMVECTOR up = XMVectorSet(0.0f,1.0f,0.0f,0.0f);

		XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

		XMStoreFloat4x4(&m_viewMatrix, view);

		XMMATRIX proj = XMLoadFloat4x4(&m_projMatrix);

		XMMATRIX viewProj = XMMatrixMultiply(view,proj);
		auto determinant = XMMatrixDeterminant(viewProj);
		XMMATRIX invViewProj = XMMatrixInverse(&determinant, viewProj);


		CameraViewConstant cvc;
		XMStoreFloat4x4(&cvc.ViewMatrix, XMMatrixTranspose(view));
		XMStoreFloat4x4(&cvc.ProjMatrix, XMMatrixTranspose(proj));
		XMStoreFloat4x4(&cvc.ViewProjMatrix, XMMatrixTranspose(viewProj));
		XMStoreFloat4x4(&cvc.InvViewProjMatrix, XMMatrixTranspose(invViewProj));

		m_cameraViewConstants.CopyData(0, cvc);
	}

	void Camera::Render(const D3DCommandContext& commandContext, const D3DPipelineStateManager& pipelineManager,
		const std::string& pipelineName)
	{
		auto cmdList = commandContext.GetCommandList();

		auto pso = pipelineManager.GetPipelineState(pipelineName);

		auto rootSig = pipelineManager.GetRootSignature(pipelineName);

		cmdList->SetGraphicsRootSignature(rootSig);

		cmdList->SetPipelineState(pso);

		cmdList->SetGraphicsRootConstantBufferView(m_rootParamSlot, m_cameraViewConstants.GetGPUVirtualAddress());

	}

	void Camera::Resize(std::uint16_t width, std::uint16_t height)
	{
		XMMATRIX p = XMMatrixPerspectiveFovLH(0.25f * XM_PI, static_cast<float>(width) / static_cast<float>(height), 1.0f,1000.0f);
		XMStoreFloat4x4(&m_projMatrix, p);
	}
}
