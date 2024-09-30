#include "Material.h"

namespace BINDU
{
	Material::Material(XMFLOAT4 albedo, XMFLOAT3 fresnel, float roughness) : m_diffuseAlbedo(albedo),m_fresnelR0(fresnel),m_roughness(roughness)
	{
	}

	Material::~Material()
	{
		if(auto parentAllocator = m_matCbAllocation.ParentAllocator.lock())
		{
			parentAllocator->Free(m_matCbAllocation.Index, 1);
		}
	}

	void Material::Update()
	{
		if(m_isDirty)
		{
			if(m_dityFrames == 0)
			{
				m_isDirty = false;
				return;
			}

			m_materialConstants.DiffuseAlbedo = m_diffuseAlbedo;
			m_materialConstants.FresnelR0 = m_fresnelR0;
			m_materialConstants.Roughness = m_roughness;
			m_materialConstants.MatTransform = m_matTransform;

			--m_dityFrames;
		}
	}

	void Material::SetConstantRootParameterIndex(std::uint16_t index)
	{
		m_rootParameterIndex = index;
	}

	std::uint16_t Material::GetConstantRootParameterIndex() const
	{
		return m_rootParameterIndex;
	}

	void Material::SetCbAllocation(const CbAllocation& cbAllocation)
	{
		m_matCbAllocation = cbAllocation;
	}

	CbAllocation* Material::GetCbAllocation()
	{
		return &m_matCbAllocation;
	}

	void Material::SetDiffuseAlbedo(XMFLOAT4 albedo)
	{
		m_diffuseAlbedo = albedo;
		m_isDirty = true;
	}

	void Material::SetFresnelR0(XMFLOAT3 fresnel)
	{
		m_fresnelR0 = fresnel;
		m_isDirty = true;
	}

	void Material::SetRoughness(float roughness)
	{
		m_roughness = roughness;
		m_isDirty = true;
	}

	void Material::SetDirty(bool dirty)
	{
		m_isDirty = dirty;
	}

	void Material::SetTransform(const XMFLOAT4X4& transform)
	{
		m_matTransform = transform;
		m_isDirty = true;
	}

	XMFLOAT4X4 Material::GetTransform4x4() const
	{
		return m_matTransform;
	}

	XMMATRIX Material::GetTransform() const
	{
		return XMLoadFloat4x4(&m_matTransform);
	}

	MaterialConstants Material::getMaterialConstants() const
	{
		return m_materialConstants;
	}

	bool Material::IsDirty() const
	{
		return m_isDirty;
	}

	XMFLOAT4 Material::GetDiffuseAlbedo4f() const
	{
		return m_diffuseAlbedo;
	}

	XMFLOAT3 Material::GetFresnelR03f() const
	{
		return m_fresnelR0;
	}

	float Material::GetRoughness() const
	{
		return m_roughness;
	}
}
