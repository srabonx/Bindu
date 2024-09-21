#include "MeshObject.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DPipelineStateManager.h"

namespace BINDU
{
	MeshObject::MeshObject(): m_vertexBuffer(8), m_indexBuffer(DXGI_FORMAT_R16_UINT)
	{
	}

	void MeshObject::SetMesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, std::uint32_t vertexCount, std::uint32_t indexCount)
	{
		m_vertexBuffer = std::move(vertexBuffer);
		m_indexBuffer = std::move(indexBuffer);

		m_vertexCount = vertexCount;
		m_indexCount = indexCount;
	}

	void MeshObject::SetMesh(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, std::uint32_t vertexCount,
		std::uint32_t indexCount)
	{
		m_vertexBuffer = vertexBuffer;
		m_indexBuffer = indexBuffer;

		m_vertexCount = vertexCount;
		m_indexCount = indexCount;
	}

	void MeshObject::Render(const D3DCommandContext& commandContext)
	{
		UpdateConstantBuffer();

		auto cmdList = commandContext.GetCommandList();
		auto vbv  = m_vertexBuffer.GetView();
		auto ibv = m_indexBuffer.GetView();

		cmdList->IASetVertexBuffers(0,1, &vbv);
		cmdList->IASetIndexBuffer(&ibv);
		cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cmdList->SetGraphicsRootConstantBufferView(m_rootParamSlot, m_constantBuffer->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(m_indexCount, 1, 0,0,0);

	}
}
