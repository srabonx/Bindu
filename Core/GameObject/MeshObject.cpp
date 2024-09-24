#include "MeshObject.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DConstantBuffer.h"

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

	void MeshObject::SetMesh(const MeshData& meshData)
	{
		m_meshData = meshData;
	}

	void MeshObject::Render(const D3DCommandContext& commandContext)
	{
		UpdateConstantBuffer();

		auto cmdList = commandContext.GetCommandList();
		auto vbv  = m_meshData.p_VertexBuffer->GetView();
		auto ibv = m_meshData.p_IndexBuffer->GetView();

		cmdList->IASetVertexBuffers(0,1, &vbv);
		cmdList->IASetIndexBuffer(&ibv);
		cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->SetGraphicsRootConstantBufferView(m_rootParamSlot, m_constantBuffer->GetGpuAddressAt(m_cbIndex));

		cmdList->DrawIndexedInstanced(m_meshData.IndexCount, 1, m_meshData.StartIndexLocation,
			m_meshData.BaseVertexLocation, 0);

	}

	void MeshObject::Update()
	{
	}
}
