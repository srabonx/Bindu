#include "DirectXCommandList.h"

#include "../../../Debug/Assert.h"

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
		auto& rtv = m_tempRt->GetRtvAllocation();

		FLOAT clearColor[] = {r,g,b,a};

		m_d3dCommandList->ClearRenderTargetView(rtv.GetCpuHandle(), clearColor, 1, nullptr);
		m_d3dCommandList->ClearDepthStencilView(m_tempRt->GetDepthStencilViewHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 1, nullptr);
	}

	void DirectXCommandList::SetViewProjectionMatrix(const XMMATRIX& viewProjMat)
	{
	}

	void DirectXCommandList::BeginDrawOnTexture(Texture* texture)
	{
		BINDU_CORE_ASSERT(m_drawOnTextureBeginEndPair == false, "EndDrawOnTexture was not called during last frame");

		m_tempRt = static_cast<DirectXRenderTexture*>(texture);

		BINDU_CORE_ASSERT(m_tempRt != nullptr, "Invalid Render Texture!");

		m_tempRt->TransitionTo(D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_tempRt->BindAsRt();

		m_drawOnTextureBeginEndPair = true;
	}

	void DirectXCommandList::EndDrawOnTexture()
	{
		BINDU_CORE_ASSERT(m_drawOnTextureBeginEndPair == true, "BeginDrawOnTexture was not called prior calling EndDrawOnTexture!");

		m_tempRt->TransitionTo(D3D12_RESOURCE_STATE_COPY_SOURCE);

		m_tempRt = nullptr;

		m_drawOnTextureBeginEndPair = false;
	}

	void DirectXCommandList::CopyRenderTextureDataToBackBuffer(Texture* renderTexture)
	{
		auto rt = static_cast<DirectXRenderTexture*>(renderTexture);

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_COPY_DEST);

		m_d3dCommandList->CopyResource(m_swapChainRt->GetResource(), rt->GetResource());

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	void DirectXCommandList::BeginRender()
	{
		BINDU_CORE_ASSERT(m_renderBeginEndPair == false, "EndRender was not called during last frame");

		m_swapChainRt = m_context->GetCurrentSwapChainRT();

		m_swapChainRt->BindAsRt();

		m_renderBeginEndPair = true;
	}

	void DirectXCommandList::BindShader(Shader* shader)
	{
		m_tempShader = static_cast<DirectXShader*>(shader);

		BINDU_CORE_ASSERT(m_tempShader != nullptr, "Invalid Shader!");

		m_tempShader->Bind();
	}

	void DirectXCommandList::BindTexture(int slot, Texture* texture)
	{
		m_tempShader->BindTexture(slot, texture);
	}

	void DirectXCommandList::DrawMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
	{

	}

	void DirectXCommandList::EndRender()
	{
		BINDU_CORE_ASSERT(m_renderBeginEndPair == true, "BeginRender was not called prior calling EndRender");

		m_swapChainRt->TransitionTo(D3D12_RESOURCE_STATE_PRESENT);

		m_swapChainRt = nullptr;

		m_renderBeginEndPair = false;
	}
}
