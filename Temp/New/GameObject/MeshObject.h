#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <cstdint>
#include <string>
#include "Buffers.h"
#include "GameObject.h"
#include "MeshManager.h"
#include "Material.h"

namespace BINDU
{
	class Texture;
	class D3DCommandContext;
	class D3DPipelineStateManager;

	class MeshObject : public GameObject
	{
	public:
		MeshObject() = default;

		MeshObject(MeshObject&& rhs) = default;

		MeshObject& operator = (MeshObject&& rhs) = default;

		virtual ~MeshObject();

		void			SetCbAllocation(const CbAllocation& allocation);

		void			SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot);

		void			SetTextureConstantRootParameterSlot(std::uint8_t rootParamSlot);

		void			SetMesh(const MeshData& meshData);

		void			SetMaterial(const std::shared_ptr<Material>& materialData);

		void			SetMaterialDynamic(const std::shared_ptr<Material>& materialData);

		void			SetTexture(const std::shared_ptr<Texture>& texture);

		virtual void	Render(D3DCommandContext& commandContext, const UploadBuffer* constantBuffer);

		virtual void	Update();

		ObjectConstants* GetConstantData();

		Material*		GetMaterialComponent() const;

		MeshData*		GetMeshData();

		std::uint8_t	GetRootParameterSlot() const;

		CbAllocation*	GetCBAllocation();

		Transform*		GetTextureTransformComponent();

	protected:
		void			UpdateConstantBuffer(const UploadBuffer* constantBuffer);

	protected:
		MeshData							m_meshData;

		std::shared_ptr<Material>			m_materialComponent{ nullptr };

		ObjectConstants						m_constantData;

		std::shared_ptr<Texture>			m_texture{ nullptr };

		std::uint8_t						m_rootParamSlot{ 0 };

		std::uint8_t						m_textureRootParamSlot{ 0 };

		CbAllocation						m_cbAllocation;

		Transform							m_textureTransformComponent;

		bool								m_hasDynamicMaterial{ false };

	};
}

#endif