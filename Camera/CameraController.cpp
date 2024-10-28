#include "CameraController.h"

#include "Camera.h"
#include "../Debug/Assert.h"

namespace BINDU
{
	CameraController::CameraController(Camera* camera) : m_camera(camera)
	{
	}

	void CameraController::SetCamera(Camera* camera)
	{
		BINDU_CORE_ASSERT(camera, "Invalid camera");
		m_camera = camera;
	}

	void CameraController::SetPosition(const XMFLOAT3& position)
	{
		m_position = position;

		ReticulateViewMatrix();
	}

	void CameraController::Orbit(float deltaYaw, float deltaPitch)
	{
		RotateY(deltaYaw);
		Pitch(deltaPitch);
	}

	void CameraController::UpdateViewMatrix()
	{
	}

	void CameraController::StrafeX(float d)
	{
		// Move the camera left of right based on the right vector
		XMVECTOR right = XMLoadFloat3(&m_right);
		XMVECTOR pos = XMLoadFloat3(&m_position);

		pos = XMVectorMultiplyAdd(XMVectorReplicate(d), right, pos);

		XMStoreFloat3(&m_position, pos);

		ReticulateViewMatrix();
	}

	void CameraController::StrafeY(float d)
	{
		// Move the camera up or down based on the up vector
		XMVECTOR up = XMLoadFloat3(&m_up);
		XMVECTOR pos = XMLoadFloat3(&m_position);

		pos = XMVectorMultiplyAdd(XMVectorReplicate(d), up, pos);

		XMStoreFloat3(&m_position, pos);

		ReticulateViewMatrix();
	}

	void CameraController::Walk(float d)
	{
		// Move the camera forward or backward based on the look vector
		XMVECTOR look = XMLoadFloat3(&m_look);
		XMVECTOR pos = XMLoadFloat3(&m_position);

		pos = XMVectorMultiplyAdd(XMVectorReplicate(d), look, pos);

		XMStoreFloat3(&m_position, pos);

		ReticulateViewMatrix();
	}

	void CameraController::Pitch(float angle)
	{
		// Rotate up or down
		m_pitch += angle;

		// Limit to pitch to avoid gimbal lock
		if (m_pitch > XM_PIDIV2) m_pitch = XM_PIDIV2;
		if (m_pitch < -XM_PIDIV2) m_pitch = -XM_PIDIV2;

		ReticulateViewMatrix();
	}

	void CameraController::RotateY(float angle)
	{
		// Rotate left or right (Y axis) 
		m_yaw += angle;

		ReticulateViewMatrix();
	}

	void CameraController::Roll(float angle)
	{
		m_roll += angle;

		ReticulateViewMatrix();
	}

	const XMFLOAT3 CameraController::GetPosition() const
	{
		return m_position;
	}

	const XMFLOAT4X4& CameraController::GetViewProjection4x4() const
	{

		auto viewProj = XMMatrixTranspose(GetViewProjection());


		XMFLOAT4X4 viewProj4x4{};
		XMStoreFloat4x4(&viewProj4x4, viewProj);

		return viewProj4x4;
	}

	const XMMATRIX& CameraController::GetProjection() const
	{
		BINDU_CORE_ASSERT(m_perspectiveCamera, "No valid camera is set");

		return m_camera->GetProjection();
	}

	const XMMATRIX& CameraController::GetView() const
	{
		auto pos = XMLoadFloat3(&m_position);
		auto look = XMLoadFloat3(&m_look);
		auto right = XMLoadFloat3(&m_right);
		auto up = XMLoadFloat3(&m_up);

		auto focusPos = XMVectorAdd(pos, look);

		return XMMatrixLookAtLH(pos, focusPos, up);
	}

	const XMMATRIX& CameraController::GetViewProjection() const
	{
		auto view = GetView();
		auto proj = GetProjection();

		return XMMatrixMultiply(view, proj);
	}

	void CameraController::ReticulateViewMatrix()
	{
		// Calculate new look vector based on yaw and pitch angles
		auto rotationMat = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
		auto lookVec = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMat);
		auto rightVec = XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotationMat);
		XMVECTOR upVec = XMVector3Cross(lookVec, rightVec);

		XMStoreFloat3(&m_look, lookVec);
		XMStoreFloat3(&m_right, rightVec);
		XMStoreFloat3(&m_up, upVec);
	}
}