#include "MeshObject.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DConstantBuffer.h"
#include "../Resources/Texture.h"

namespace BINDU
{

	//void MeshObject::SetMesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, std::uint32_t vertexCount, std::uint32_t indexCount)
	//{
	//	m_vertexBuffer = std::move(vertexBuffer);
	//	m_indexBuffer = std::move(indexBuffer);

	//	m_vertexCount = vertexCount;
	//	m_indexCount = indexCount;
	//}

	//void MeshObject::SetMesh(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, std::uint32_t vertexCount,
	//	std::uint32_t indexCount)
	//{
	//	m_vertexBuffer = vertexBuffer;
	//	m_indexBuffer = indexBuffer;

	//	m_vertexCount = vertexCount;
	//	m_indexCount = indexCount;
	//}


	MeshObject::~MeshObject()
	{
		if(auto allocator = m_cbAllocation.ParentAllocator.lock())
		{
			allocator->Free(m_cbAllocation.Index, 1);
		}
	}

	void MeshObject::SetCbAllocation(const CbAllocation& allocation)
	{
		m_cbAllocation = allocation;
	}

	CbAllocation* MeshObject::GetCBAllocation()
	{
		return &m_cbAllocation;
	}

	Transform* MeshObject::GetTextureTransformComponent()
	{
		return &m_textureTransformComponent;
	}

	void MeshObject::SetObjectConstantRootParameterSlot(std::uint8_t RootParamSlot)
	{
		m_rootParamSlot = RootParamSlot;
	}

	void MeshObject::SetTextureConstantRootParameterSlot(std::uint8_t rootParamSlot)
	{
		m_textureRootParamSlot = rootParamSlot;
	}

	void MeshObject::SetMesh(const MeshData& meshData)
	{
		m_meshData = meshData;
	}

	void MeshObject::SetMaterial(const std::shared_ptr<Material>& materialData)
	{
		m_materialComponent = materialData;
	}

	void MeshObject::SetMaterialDynamic(const std::shared_ptr<Material>& materialData)
	{
		m_hasDynamicMaterial = true;
		m_materialComponent = std::move(materialData);
	}


	void MeshObject::SetTexture(const std::shared_ptr<Texture>& texture)
	{
		m_texture = texture;
	}

	Material* MeshObject::GetMaterialComponent() const
	{
		return m_materialComponent.get();
	}

	MeshData* MeshObject::GetMeshData()
	{
		return &m_meshData;
	}

	std::uint8_t MeshObject::GetRootParameterSlot() const
	{
		return m_rootParamSlot;
	}

	void MeshObject::UpdateConstantBuffer(const UploadBuffer* constantBuffer)
	{
/*		if(m_transformComponent.IsDirty() || m_materialComponent.IsDirty())
		{
			if(m_transformComponent.IsDirty())
			{
				m_constantData.WorldMatrix = m_transformComponent.GetTransform4x4f();
			}

			if(m_materialComponent.IsDirty())
			{
				m_constantData.DiffuseAlbedo = m_materialComponent.GetDiffuseAlbedo4f();
				m_constantData.FresnelR0 = m_materialComponent.GetFresnelR03f();
				m_constantData.Roughness = m_materialComponent.GetRoughness();

				m_materialComponent.SetDirty(false);
			}
		} */


		if (m_transformComponent.IsDirty() || m_textureTransformComponent.IsDirty())
		{
			m_constantData.WorldMatrix = m_transformComponent.GetTransform4x4f();
			m_constantData.TextureTransform = m_textureTransformComponent.GetTransform4x4f();
			constantBuffer->CopyData(m_cbAllocation.Index, &m_constantData);
		}

	}


	void MeshObject::Render(D3DCommandContext& commandContext, const UploadBuffer* constantBuffer)
	{

		UpdateConstantBuffer(constantBuffer);

		auto cmdList = commandContext.GetCommandList();

		auto vbv  = m_meshData.p_VertexBuffer->GetView();
		auto ibv = m_meshData.p_IndexBuffer->GetView();

		cmdList->IASetVertexBuffers(0,1, &vbv);
		cmdList->IASetIndexBuffer(&ibv);
		cmdList->IASetPrimitiveTopology(m_meshData.PrimitiveTopology);

		if (m_texture)
			cmdList->SetGraphicsRootDescriptorTable(m_textureRootParamSlot, m_texture->GetHeapAllocation()->GetGpuHandle());

		cmdList->SetGraphicsRootConstantBufferView(m_rootParamSlot, constantBuffer->GetGPUVirtualAddressAt(m_cbAllocation.Index));

		cmdList->DrawIndexedInstanced(m_meshData.IndexCount, 1, m_meshData.StartIndexLocation,
			m_meshData.BaseVertexLocation, 0);

	}

	void MeshObject::Update()
	{
		m_transformComponent.Update();
		m_textureTransformComponent.Update();

		if (m_hasDynamicMaterial)
			m_materialComponent->Update();

	}

	ObjectConstants* MeshObject::GetConstantData()
	{
		return &m_constantData;
	}
}
