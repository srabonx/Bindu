#include "Renderable.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/Utility/D3DUtillity.h"
#include "../Renderer/DirectX/Utility/DescriptorHeapManager.h"
#include "../Renderer/DirectX/Utility/FlyFrame.h"


namespace BINDU
{
	Renderable::Renderable(std::uint32_t indexCount, std::uint32_t startIndexLocation, int baseVertexLocation, FlyFrame* parentFrame) :
		m_indexCount(indexCount), m_startIndexLocation(startIndexLocation), m_baseVertexLocation(baseVertexLocation), m_parentFlyFrame(parentFrame)
	{
		DirectX::XMStoreFloat4x4(&m_worldMatrix, DirectX::XMMatrixIdentity());

	}

	void Renderable::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_indexBuffer = indexBuffer;
	}

	void Renderable::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_vertexBuffer = vertexBuffer;
	}

	void Renderable::SetPso(const ComPtr<ID3D12PipelineState>& pso)
	{
		m_pso = pso;
	}

	XMFLOAT4X4& Renderable::GetWorldMatrix()
	{
		return m_worldMatrix;
	}

	std::uint32_t Renderable::GetCbIndex() const
	{
		return m_cbIndex;
	}

	void Renderable::Initialize(std::uint32_t cbIndex)
	{
		m_cbIndex = cbIndex;

		auto d3dDevice = m_parentFlyFrame->GetParentDevice()->GetD3DDevice();

		auto cbvGpuHeap = m_parentFlyFrame->GetParentDevice()->GetCbvSrvUavGpuHeap();

		m_descriptorHeapAllocation = cbvGpuHeap->Allocate(m_parentFlyFrame->GetFrameCount());

		for (int i = 0; i < m_parentFlyFrame->GetFrameCount(); i++)
		{

			auto cbByteSize = m_parentFlyFrame->GetFrame(i).ObjectCb->GetElementByteSize();

			auto cbAddress = m_parentFlyFrame->GetFrame(i).ObjectCb->GetGPUVirtualAddress();

			cbAddress += cbIndex * cbByteSize;

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbv;
			cbv.BufferLocation = cbAddress;
			cbv.SizeInBytes = cbByteSize;

			d3dDevice->CreateConstantBufferView(&cbv, m_descriptorHeapAllocation.GetCpuHandle(i));
		}
	}

	void Renderable::UpdateConstantData()
	{
		XMMATRIX translation = XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
		XMMATRIX rotation = XMMatrixRotationX(m_rotation.x);
		rotation *= XMMatrixRotationY(m_rotation.y);
		rotation *= XMMatrixRotationZ(m_rotation.z);
		XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

		XMMATRIX world = translation * rotation * scale;

		XMStoreFloat4x4(&m_worldMatrix, XMMatrixTranspose(world));
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

		auto gpuAddress = m_parentFlyFrame->GetCurrentFrame().ObjectCb->GetGPUVirtualAddress();
		gpuAddress += m_cbIndex * m_parentFlyFrame->GetCurrentFrame().ObjectCb->GetElementByteSize();
		cmdList->SetGraphicsRootConstantBufferView(shaderRegister, gpuAddress);

		cmdList->DrawIndexedInstanced(m_indexCount, 1,
			m_startIndexLocation, m_baseVertexLocation, 0);

	}

}
