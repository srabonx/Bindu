#include "Transform.h"

namespace BINDU
{


	void XM_CALLCONV Transform::SetRotation(FXMVECTOR rotation)
	{
		XMStoreFloat3(&m_rotation, rotation);
		m_isDirty = true;
	}

	void XM_CALLCONV Transform::SetScale(FXMVECTOR scale)
	{
		XMStoreFloat3(&m_scale, scale);
		m_isDirty = true;
	}

	void XM_CALLCONV Transform::SetTranslation(FXMVECTOR translation)
	{
		XMStoreFloat3(&m_translation, translation);
		m_isDirty = true;
	}

	void Transform::SetRotation(float x, float y, float z)
	{
		m_rotation = {x,y,z};
		m_isDirty = true;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		m_scale = {x,y,z};
		m_isDirty = true;
	}

	void Transform::SetTranslation(float x, float y, float z)
	{
		m_translation = {x,y,z};
		m_isDirty = true;
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

	void Transform::SetDirty(bool dirty)
	{
		m_isDirty = dirty;
	}

	bool Transform::IsDirty() const
	{
		return m_isDirty;
	}
}
