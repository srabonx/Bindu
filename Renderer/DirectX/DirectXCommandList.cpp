#include "DirectXCommandList.h"

#include "D3DCommandContext.h"
#include "D3DUtillity.h"

#include "../../Debug/Assert.h"

#include "../../Geometry/MeshData.h"

namespace BINDU
{
	DirectXCommandList::DirectXCommandList(DirectXGraphicsContext* context) : m_context(context)
	{
		m_d3dCommandList = context->m_commandContext->GetCommandList();
	}

	void DirectXCommandList::SetViewPort(float width, float height)
	{
		m_viewPort = { 0.0f,0.0f,width,height,0.0f,1.0f };
		m_d3dCommandList->RSSetViewports(1, &m_viewPort);
	}

	void DirectXCommandList::SetScissorRect(int x, int y, int width, int height)
	{
		m_scissorRect = { 0,0,width,height };

		m_d3dCommandList->RSSetScissorRects(1, &m_scissorRect);
	}

	void DirectXCommandList::Clear(float r, float g, float b, float a)
	{
		auto& rtv = m_currBoundRt->GetRtvAllocation();

		FLOAT clearColor[] = {r,g,b,a};

		m_d3dCommandList->ClearRenderTargetView(rtv.GetCpuHandle(), clearColor, 0, nullptr);
		m_d3dCommandList->ClearDepthStencilView(m_currBoundRt->GetDepthStencilViewHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);
	}

	void DirectXCommandList::BeginDrawOnTexture(Texture* texture)
	{
		BINDU_CORE_ASSERT(m_drawOnTextureBeginEndPair == false, "EndDrawOnTexture was not called during last frame");

		m_tempRt = static_cast<DirectXRenderTexture*>(texture);

		m_currBoundRt = m_tempRt;

		BINDU_CORE_ASSERT(m_tempRt != nullptr, "Invalid Render Texture!");

		m_tempRt->BindAsRt();

		m_drawOnTextureBeginEndPair = true;
	}

	void DirectXCommandList::EndDrawOnTexture()
	{
		BINDU_CORE_ASSERT(m_drawOnTextureBeginEndPair == true, "BeginDrawOnTexture was not called prior calling EndDrawOnTexture!");

		m_tempRt->TransitionTo(D3D12_RESOURCE_STATE_COPY_SOURCE);

		m_tempRt = nullptr;

		m_currBoundRt = nullptr;

		m_drawOnTextureBeginEndPair = false;
	}

	void DirectXCommandList::CopyRenderTextureDataToBackBuffer(Texture* renderTexture)
	{
		auto rt = static_cast<DirectXRenderTexture*>(renderTexture);

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_COPY_DEST);


/*		CD3DX12_TEXTURE_COPY_LOCATION dest;
		dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dest.pResource = m_swapChainRt->GetResource();
		dest.SubresourceIndex = 0;

		CD3DX12_TEXTURE_COPY_LOCATION src;
		src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		src.pResource = rt->GetResource();
		src.SubresourceIndex = 0;

		m_d3dCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr); */


		m_d3dCommandList->CopyResource(m_swapChainRt->GetResource(), rt->GetResource());

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_RENDER_TARGET);
		
	}

	void DirectXCommandList::BeginRender()
	{
		BINDU_CORE_ASSERT(m_renderBeginEndPair == false, "EndRender was not called during last frame");

		m_swapChainRt = m_context->GetCurrentSwapChainRT();

		m_currBoundRt = m_swapChainRt;

		m_swapChainRt->BindAsRt();

		m_renderBeginEndPair = true;
	}

	bool DirectXCommandList::BindShader(Shader* shader)
	{
		if (m_currBoundShader == shader)
			return false;

		m_currBoundShader = static_cast<DirectXShader*>(shader);

		BINDU_CORE_ASSERT(m_currBoundShader != nullptr, "Invalid Shader!");

		m_currBoundShader->Bind();

		return true;
	}


	void DirectXCommandList::BindTexture(int slot, Texture* texture)
	{
		m_currBoundShader->BindTexture(slot, texture);
	}

	void DirectXCommandList::DrawMesh(const MeshData& meshData)
	{
		meshData.p_VertexBuffer->Bind();
		meshData.p_IndexBuffer->Bind();

		m_d3dCommandList->IASetPrimitiveTopology(D3DUtility::GetPrimitiveTopologyFromPrimitiveType(meshData.Type));

		m_d3dCommandList->DrawIndexedInstanced(meshData.IndexCount, 1, meshData.StartIndexLocation,
			static_cast<int>(meshData.BaseVertexLocation), 0);
	}


	void DirectXCommandList::EndRender()
	{
		BINDU_CORE_ASSERT(m_renderBeginEndPair == true, "BeginRender was not called prior calling EndRender");

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_PRESENT);

		m_swapChainRt = nullptr;

		m_currBoundRt = nullptr;

		m_currBoundShader = nullptr;

		m_renderBeginEndPair = false;
	}
}
