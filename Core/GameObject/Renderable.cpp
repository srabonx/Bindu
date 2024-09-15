#include "Renderable.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/Utility/D3DUtillity.h"
#include "../Renderer/DirectX/Utility/DescriptorHeapManager.h"


namespace BINDU
{
	Renderable::Renderable(std::uint32_t indexCount, std::uint32_t startIndexLocation, int baseVertexLocation, FlyFrame* parentFrame) :
		m_indexCount(indexCount), m_startIndexLocation(startIndexLocation), m_baseVertexLocation(baseVertexLocation), m_parentFlyFrame(parentFrame)
	{
	}

	void Renderable::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_indexBuffer = indexBuffer;
	}

	void Renderable::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_vertexBuffer = vertexBuffer;
	}

	void Renderable::Update()
	{

	}

	void Renderable::Draw(const D3DCommandContext& commandContext, std::uint32_t shaderRegister)
	{
		auto cmdList = commandContext.GetCommandList();

		cmdList->SetPipelineState(m_pso.Get());

		cmdList->IASetPrimitiveTopology(m_primitiveTopology);

		auto indexView = m_indexBuffer->GetView();

		cmdList->IASetIndexBuffer(&indexView);

		auto vertexView = m_vertexBuffer->GetView();

		cmdList->IASetVertexBuffers(0, 1, &vertexView);

		cmdList->SetGraphicsRootDescriptorTable(shaderRegister, m_descriptorHeapAllocation->GetGpuHandle());

		cmdList->DrawIndexedInstanced(m_indexCount, 1,
			m_startIndexLocation, m_baseVertexLocation, 0);

	}

}
