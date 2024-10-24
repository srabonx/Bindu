#ifndef BINDU_DIRECTX_COMMANDLIST_H
#define BINDU_DIRECTX_COMMANDLIST_H

#include "DirectXGraphicsContext.h"
#include "DirectXRenderTexture.h"
#include "../CommandList.h"
#include "DirectXShader.h"


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

		void BeginRender() override;

		void BeginDrawOnTexture(Texture* texture) override;

		void EndDrawOnTexture() override;

		void CopyRenderTextureDataToBackBuffer(Texture* renderTexture) override;

		bool BindShader(Shader* shader) override;

		void BindTexture(int slot, Texture* texture) override;

		void DrawMesh(const MeshData& meshData) override;

		void EndRender() override;

	private:

		DirectXGraphicsContext*		m_context{ nullptr };

		ComPtr<ID3D12GraphicsCommandList>	m_d3dCommandList{ nullptr };

		DirectXRenderTexture*		m_currBoundRt{ nullptr };

		DirectXRenderTexture*		m_tempRt{ nullptr };

		DirectXRenderTexture*		m_swapChainRt{ nullptr };

		DirectXShader*				m_currBoundShader{ nullptr };

		D3D12_VIEWPORT				m_viewPort;
		D3D12_RECT					m_scissorRect;

		bool						m_renderBeginEndPair{ false };

		bool						m_drawOnTextureBeginEndPair{ false };
	};
}

#endif