#include "Transform.h"

namespace BINDU
{
	void Transform::Update()
	{
		if(m_isDirty)
		{
			if (m_dirtyFrameCount == 0)
			{
				m_isDirty = false;
				return;
			}

			if (!m_setTransform)
			{
				auto rotationVec = GetRotation();
				auto scaleVec = GetScale();
				auto translationVec = GetTranslation();

				auto rotationMat = XMMatrixRotationRollPitchYawFromVector(rotationVec);
				auto scaleMat = XMMatrixScalingFromVector(scaleVec);
				auto translationMat = XMMatrixTranslationFromVector(translationVec);


				auto transform = rotationMat * scaleMat * translationMat;//XMMatrixAffineTransformation(scaleVec, XMVectorZero(), rotationVec, translationVec);

				XMStoreFloat4x4(&m_transform, XMMatrixTranspose(transform));
			}

			--m_dirtyFrameCount;
		}
	}

	void XM_CALLCONV Transform::SetRotation(FXMVECTOR rotation)
	{
		XMStoreFloat3(&m_rotation, rotation);
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void XM_CALLCONV Transform::SetScale(FXMVECTOR scale)
	{
		XMStoreFloat3(&m_scale, scale);
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void XM_CALLCONV Transform::SetTranslation(FXMVECTOR translation)
	{
		XMStoreFloat3(&m_translation, translation);
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void XM_CALLCONV Transform::SetTransform(FXMMATRIX transform)
	{
		XMVECTOR scale, rotation, translation = {};

		XMMatrixDecompose(&scale, &rotation, &translation, transform);

		XMStoreFloat3(&m_scale, scale);
		XMStoreFloat3(&m_rotation, rotation);
		XMStoreFloat3(&m_translation, translation);

		XMStoreFloat4x4(&m_transform, XMMatrixTranspose(transform));

		m_setTransform = true;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void Transform::SetRotation(float x, float y, float z)
	{
		m_rotation = {x,y,z};
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		m_scale = { x,y,z };
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void Transform::SetTranslation(float x, float y, float z)
	{
		m_translation = { x,y,z };
		m_setTransform = false;
		m_isDirty = true;
		m_dirtyFrameCount = 3;
	}

	void Transform::SetTransform(const XMFLOAT4X4& transform)
	{
		m_transform = transform;
		m_setTransform = true;
	}

	XMVECTOR Transform::GetRotation() const
	{
		return XMLoadFloat3(&m_rotation);
	}

	XMVECTOR Transform::GetScale() const
	{
		return XMLoadFloat3(&m_scale);
	}

	XMVECTOR Transform::GetTranslation() const
	{
		return XMLoadFloat3(&m_translation);
	}

	XMMATRIX Transform::GetTransform() const
	{
		return XMLoadFloat4x4(&m_transform);
	}

	XMFLOAT3 Transform::GetRotation3f() const
	{
		return m_rotation;
	}

	XMFLOAT3 Transform::GetScale3f() const
	{
		return m_scale;
	}

	XMFLOAT3 Transform::GetTranslation3f() const
	{
		return m_translation;
	}

	XMFLOAT4X4 Transform::GetTransform4x4f() const
	{
		return m_transform;
	}

	XMVECTOR Transform::GetForwardVector() const
	{
		auto forward = GetForwardVector3f();

		return XMLoadFloat3(&forward);
	}

	XMFLOAT3 Transform::GetForwardVector3f() const
	{
		XMFLOAT3 forward;

		XMMATRIX transform = GetTransform();

		forward.x = transform.r[2].m128_f32[0];	// X component
		forward.y = transform.r[2].m128_f32[1];	// Y component
		forward.z = transform.r[2].m128_f32[2];	// Z component

		return forward;
	}

	void Transform::SetDirty(bool dirty)
	{
		m_isDirty = dirty;
	}

	bool Transform::IsDirty() const
	{
		return m_isDirty;
	}
}
