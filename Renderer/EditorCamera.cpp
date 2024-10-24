#define NOMINMAX
#include "EditorCamera.h"

#include "../Event/EventManager.h"
#include "../Input/InputManager.h"

namespace BINDU
{
	void EditorCamera::OnUpdate()
	{
		if (m_viewDirty)
		{
			XMVECTOR right = XMLoadFloat3(&m_right);
			XMVECTOR up = XMLoadFloat3(&m_up);
			XMVECTOR look = XMLoadFloat3(&m_look);
			XMVECTOR pos = XMLoadFloat3(&m_position);

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


	XMVECTOR EditorCamera::GetRight() const
	{
		return XMLoadFloat3(&m_right);
	}

	XMFLOAT3 EditorCamera::GetRight3f() const
	{
		return m_right;
	}

	XMVECTOR EditorCamera::GetUp() const
	{
		return XMLoadFloat3(&m_up);
	}

	XMFLOAT3 EditorCamera::GetUp3d() const
	{
		return m_up;
	}

	XMVECTOR EditorCamera::GetLook() const
	{
		return XMLoadFloat3(&m_look);
	}

	XMFLOAT3 EditorCamera::GetLook3f() const
	{
		return m_look;
	}

	float EditorCamera::GetNearZ() const
	{
		return m_nearZ;
	}

	float EditorCamera::GetFarZ() const
	{
		return m_farZ;
	}

	float EditorCamera::GetAspect() const
	{
		return m_aspect;
	}

	float EditorCamera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();

		return 2.0f * atan(halfWidth / m_nearZ);
	}

	float EditorCamera::GetFovY() const
	{
		return m_fovY;
	}

	float EditorCamera::GetNearWindowWidth() const
	{
		return m_aspect * m_nearWindowHeight;
	}

	float EditorCamera::GetNearWindowHeight() const
	{
		return m_nearWindowHeight;
	}

	float EditorCamera::GetFarWindowWidth() const
	{
		return m_aspect * m_farWindowHeight;
	}

	float EditorCamera::GetFarWindowHeight() const
	{
		return m_farWindowHeight;
	}

	void EditorCamera::SetLens(float fovY, float aspect, float nearZ, float farZ)
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

	void EditorCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
	{
		XMVECTOR look = XMVector3Normalize(XMVectorSubtract(target, pos));
		XMVECTOR right = XMVector3Cross(worldUp, look);
		XMVECTOR up = XMVector3Cross(look, right);

		XMStoreFloat3(&m_position, pos);
		XMStoreFloat3(&m_look, look);
		XMStoreFloat3(&m_right, right);
		XMStoreFloat3(&m_up, up);

		m_viewDirty = true;
	}

	void EditorCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
	{
		XMVECTOR p = XMLoadFloat3(&pos);
		XMVECTOR t = XMLoadFloat3(&target);
		XMVECTOR u = XMLoadFloat3(&up);

		LookAt(p, t, u);

	}

	XMMATRIX EditorCamera::GetView() const
	{
		return XMLoadFloat4x4(&m_viewMatrix);
	}

	XMMATRIX EditorCamera::GetProj() const
	{
		return XMLoadFloat4x4(&m_projMatrix);
	}

	XMMATRIX EditorCamera::GetViewProj() const
	{
		return XMLoadFloat4x4(&m_viewProjMatrix);
	}

	XMFLOAT4X4 EditorCamera::GetViewProj4x4f() const
	{
		return m_viewProjMatrix;
	}

	XMFLOAT4X4 EditorCamera::GetView4x4f() const
	{
		return m_viewMatrix;
	}

	XMFLOAT4X4 EditorCamera::GetProj4x4f() const
	{
		return m_projMatrix;
	}

	void EditorCamera::StrafeX(float d)
	{
		// position += d*m_right;
		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR r = XMLoadFloat3(&m_right);
		XMVECTOR p = XMLoadFloat3(&m_position);

		auto newPos = XMVectorMultiplyAdd(s, r, p);

		XMStoreFloat3(&m_position, newPos);

		m_viewDirty = true;
	}

	void EditorCamera::StrafeY(float d)
	{
		// position += d*m_right;
		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR u = XMLoadFloat3(&m_up);
		XMVECTOR p = XMLoadFloat3(&m_position);

		auto newPos = XMVectorMultiplyAdd(s, -u, p);

		XMStoreFloat3(&m_position, newPos);

		m_viewDirty = true;
	}

	void EditorCamera::Walk(float d)
	{
		// position += d * look;

		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR l = XMLoadFloat3(&m_look);
		XMVECTOR p = XMLoadFloat3(&m_position);

		auto newPos = XMVectorMultiplyAdd(s, l, p);

		XMStoreFloat3(&m_position, newPos);

		m_viewDirty = true;
	}

	void EditorCamera::Pitch(float angle)
	{
		// Rotate Up and Look vector about the right vector

		XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

		XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
		XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), r));

		m_viewDirty = true;
	}

	void EditorCamera::RotateY(float angle)
	{
		// Rotate the basis vectors about the world y-axis
		XMMATRIX r = XMMatrixRotationY(angle);

		XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
		XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
		XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), r));

		m_viewDirty = true;
	}

	void EditorCamera::SetPosition(float x, float y, float z)
	{
		m_position = { x,y,z };
	}
}
