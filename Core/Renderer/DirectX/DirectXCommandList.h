#ifndef BINDU_DIRECTX_COMMANDLIST_H
#define BINDU_DIRECTX_COMMANDLIST_H

#include "D3DCommandContext.h"
#include "DirectXGraphicsContext.h"
#include "DirectXRenderTexture.h"
#include "DirectXShader.h"
#include "../CommandList.h"

namespace BINDU
{
	class DirectXCommandList : public CommandList
	{
	public:
		explicit DirectXCommandList(DirectXGraphicsContext* context);

		~DirectXCommandList() override = default;

		void SetViewPort(float width, float height) override;
		void SetScissorRect(int x, int y, int width, int height) override;
		void Clear(float r, float g, float b, float a) override;
		void SetViewProjectionMatrix(const XMMATRIX& viewProjMat) override;
		void BeginRender() override;

		void BeginDrawOnTexture(Texture* texture) override;

		void EndDrawOnTexture() override;

		void CopyRenderTextureDataToBackBuffer(Texture* rednerTexture) override;

		void BindShader(Shader* shader) override;
		void BindTexture(int slot, Texture* texture) override;
		void DrawMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) override;
		void EndRender() override;

	private:

		DirectXGraphicsContext*		m_context{ nullptr };

		ComPtr<ID3D12GraphicsCommandList>	m_d3dCommandList{ nullptr };

		DirectXRenderTexture*		m_tempRt{ nullptr };

		DirectXRenderTexture*		m_swapChainRt{ nullptr };

		DirectXShader*				m_tempShader{ nullptr };

		D3D12_VIEWPORT				m_viewPort;
		D3D12_RECT					m_scissorRect;

		bool						m_renderBeginEndPair{ false };

		bool						m_drawOnTextureBeginEndPair{ false };
	};
}

#endif