#include "Camera.h"

namespace BINDU
{

	void Camera::Update()
	{
		if(m_viewDirty)
		{
			XMVECTOR right = XMLoadFloat3(&m_right);
			XMVECTOR up = XMLoadFloat3(&m_up);
			XMVECTOR look = XMLoadFloat3(&m_look);
			XMVECTOR pos = m_transformComponent.GetTranslation();

			// Keep camera axis orthogonal to each other and of unit length
			look = XMVector3Normalize(look);
			up = XMVector3Normalize(XMVector3Cross(look, right));

			// up, look is already ortho-normal so no need to normalize cross product
			right = XMVector3Cross(up, look);

			// Fill the view matrix
			float x = -XMVectorGetX(XMVector3Dot(pos, right));
			float y = -XMVectorGetY(XMVector3Dot(pos, up));
			float z = -XMVectorGetZ(XMVector3Dot(pos, look));

			XMStoreFloat3(&m_right, right);
			XMStoreFloat3(&m_up, up);
			XMStoreFloat3(&m_look, look);

			m_viewMatrix(0, 0) = m_right.x;
			m_viewMatrix(1, 0) = m_right.y;
			m_viewMatrix(2, 0) = m_right.z;
			m_viewMatrix(3, 0) = x;

			m_viewMatrix(0, 1) = m_up.x;
			m_viewMatrix(1, 1) = m_up.y;
			m_viewMatrix(2, 1) = m_up.z;
			m_viewMatrix(3, 1) = y;

			m_viewMatrix(0, 2) = m_look.x;
			m_viewMatrix(1, 2) = m_look.y;
			m_viewMatrix(2, 2) = m_look.z;
			m_viewMatrix(3, 2) = z;

			m_viewMatrix(0, 3) = 0.0f;
			m_viewMatrix(1, 3) = 0.0f;
			m_viewMatrix(2, 3) = 0.0f;
			m_viewMatrix(3, 3) = 1.0f;


			XMMATRIX viewProj = XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix));
			XMStoreFloat4x4(&m_viewProjMatrix, XMMatrixTranspose(viewProj));

			m_viewDirty = false;
		}

	}

	void Camera::Render(const D3DCommandContext& commandContext,const UploadBuffer* constantBuffer)
	{
		UpdateConstantBuffer(constantBuffer);
	}


	XMVECTOR Camera::GetRight() const
	{
		return XMLoadFloat3(&m_right);
	}

	XMFLOAT3 Camera::GetRight3f() const
	{
		return m_right;
	}

	XMVECTOR Camera::GetUp() const
	{
		return XMLoadFloat3(&m_up);
	}

	XMFLOAT3 Camera::GetUp3d() const
	{
		return m_up;
	}

	XMVECTOR Camera::GetLook() const
	{
		return XMLoadFloat3(&m_look);
	}

	XMFLOAT3 Camera::GetLook3f() const
	{
		return m_look;
	}

	float Camera::GetNearZ() const
	{
		return m_nearZ;
	}

	float Camera::GetFarZ() const
	{
		return m_farZ;
	}

	float Camera::GetAspect() const
	{
		return m_aspect;
	}

	float Camera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();

		return 2.0f * atan(halfWidth / m_nearZ);
	}

	float Camera::GetFovY() const
	{
		return m_fovY;
	}

	float Camera::GetNearWindowWidth() const
	{
		return m_aspect * m_nearWindowHeight;
	}

	float Camera::GetNearWindowHeight() const
	{
		return m_nearWindowHeight;
	}

	float Camera::GetFarWindowWidth() const
	{
		return m_aspect * m_farWindowHeight;
	}

	float Camera::GetFarWindowHeight() const
	{
		return m_farWindowHeight;
	}

	void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ)
	{
		m_fovY = fovY;
		m_aspect = aspect;
		m_nearZ = nearZ;
		m_farZ = farZ;

		m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
		m_farWindowHeight = 2.0f * m_farZ * tanf(0.5f * m_fovY);

		XMMATRIX p = XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
		XMStoreFloat4x4(&m_projMatrix, p);

		m_viewDirty = true;
	}

	void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
	{
		XMVECTOR look = XMVector3Normalize(XMVectorSubtract(target, pos));
		XMVECTOR right = XMVector3Cross(worldUp, look);
		XMVECTOR up = XMVector3Cross(look,right);

		m_transformComponent.SetTranslation(pos);
		XMStoreFloat3(&m_look, look);
		XMStoreFloat3(&m_right, right);
		XMStoreFloat3(&m_up, up);

		m_viewDirty = true;
	}

	void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
	{
		XMVECTOR p = XMLoadFloat3(&pos);
		XMVECTOR t = XMLoadFloat3(&target);
		XMVECTOR u = XMLoadFloat3(&up);

		LookAt(p,t,u);

	}

	XMMATRIX Camera::GetView() const
	{
		return XMLoadFloat4x4(&m_viewMatrix);
	}

	XMMATRIX Camera::GetProj() const
	{
		return XMLoadFloat4x4(&m_projMatrix);
	}

	XMMATRIX Camera::GetViewProj() const
	{
		return XMLoadFloat4x4(&m_viewProjMatrix);
	}

	XMFLOAT4X4 Camera::GetViewProj4x4f() const
	{
		return m_viewProjMatrix;
	}

	XMFLOAT4X4 Camera::GetView4x4f() const
	{
		return m_viewMatrix;
	}

	XMFLOAT4X4 Camera::GetProj4x4f() const
	{
		return m_projMatrix;
	}

	void Camera::StrafeX(float d)
	{
		// position += d*m_right;
		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR r = XMLoadFloat3(&m_right);
		XMVECTOR p = m_transformComponent.GetTranslation();

		m_transformComponent.SetTranslation(XMVectorMultiplyAdd(s, r, p));

		m_viewDirty = true;
	}

	void Camera::StrafeY(float d)
	{
		// position += d*m_right;
		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR u = XMLoadFloat3(&m_up);
		XMVECTOR p = m_transformComponent.GetTranslation();

		m_transformComponent.SetTranslation(XMVectorMultiplyAdd(s, -u, p));

		m_viewDirty = true;
	}

	void Camera::Walk(float d)
	{
		// position += d * look;

		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR l = XMLoadFloat3(&m_look);
		XMVECTOR p = m_transformComponent.GetTranslation();

		m_transformComponent.SetTranslation(XMVectorMultiplyAdd(s, l, p));

		m_viewDirty = true;
	}

	void Camera::Pitch(float angle)
	{
		// Rotate Up and Look vector about the right vector

		XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

		XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
		XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), r));

		m_viewDirty = true;
	}

	void Camera::RotateY(float angle)
	{
		// Rotate the basis vectors about the world y-axis
		XMMATRIX r = XMMatrixRotationY(angle);

		XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
		XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
		XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), r));

		m_viewDirty = true;
	}
}
