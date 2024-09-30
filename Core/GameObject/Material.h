#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <DirectXMath.h>
#include "../../MathHelper/MathHelper.h"
#include "../Managers/CbAllocation.h"

namespace BINDU
{
	using namespace DirectX;

	struct MaterialConstants
	{
		//MaterialConstants() = default;
		//MaterialConstants(XMFLOAT4 albedo, XMFLOAT3 fresnel, float roughness) : DiffuseAlbedo(albedo), FresnelR0(fresnel), Roughness(roughness)
		//{
		//	
		//}

		XMFLOAT4			DiffuseAlbedo{ 1.0f,1.0f,1.0f,1.0f };

		XMFLOAT3			FresnelR0{ 0.01f,0.01f,0.01f };

		float				Roughness{ 0.25f };

		XMFLOAT4X4			MatTransform = MathHelper::Identity4x4();
	};

	class Material
	{
	public:
		Material() = default;

		Material(XMFLOAT4 albedo, XMFLOAT3 fresnel, float roughness);

		~Material();

		void			Update();

		void			SetConstantRootParameterIndex(std::uint16_t index);

		std::uint16_t	GetConstantRootParameterIndex() const;

		void			SetCbAllocation(const CbAllocation& cbAllocation);

		void			SetDiffuseAlbedo(XMFLOAT4 albedo);

		void			SetFresnelR0(XMFLOAT3 fresnel);

		void			SetRoughness(float roughness);

		void			SetDirty(bool dirty);

		void			SetTransform(const XMFLOAT4X4& transform);

		CbAllocation*	GetCbAllocation();

		XMFLOAT4		GetDiffuseAlbedo4f() const;

		XMFLOAT3		GetFresnelR03f() const;

		float			GetRoughness() const;

		bool			IsDirty() const;

		XMFLOAT4X4		GetTransform4x4() const;

		XMMATRIX		GetTransform() const;

		MaterialConstants getMaterialConstants() const;

	private:
		XMFLOAT4			m_diffuseAlbedo{ 1.0f,1.0f,1.0f,1.0f };

		XMFLOAT3			m_fresnelR0{ 0.01f,0.01f,0.01f };

		float				m_roughness{ 0.25f };

		XMFLOAT4X4			m_matTransform = MathHelper::Identity4x4();

		MaterialConstants	m_materialConstants;

		CbAllocation		m_matCbAllocation;

		std::uint16_t		m_rootParameterIndex{ 0 };

		bool				m_isDirty{ true };

		std::uint8_t		m_dityFrames{ 3 };
	};
}

#endif