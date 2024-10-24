#include "GameObject.h"

#include "../Renderer/DirectX/D3DConstantBuffer.h"

namespace BINDU
{
	GameObject::GameObject()
	{
		UuidCreate(&m_guid);
	}

	GameObject::GameObject(GameObject&& obj) noexcept
	{
		std::swap(this->m_transformComponent, obj.m_transformComponent);
	}

	GameObject& GameObject::operator=(GameObject&& obj) noexcept
	{
		std::swap(this->m_transformComponent, obj.m_transformComponent);

		return *this;
	}

	GameObject::~GameObject()
	{
	}

	Transform* GameObject::GetTransformComponent()
	{
		return &m_transformComponent;
	}

	void GameObject::SetTransform(const Transform& transform)
	{
		m_transformComponent = transform;
	}



	UUID GameObject::GetGuid() const
	{
		return m_guid;
	}

}
