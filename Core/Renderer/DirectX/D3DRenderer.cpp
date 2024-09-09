#include "D3DRenderer.h"

namespace BINDU
{
	D3DRenderer::D3DRenderer(Win32Window* window) : m_renderTexture(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT, 2, 
		{1,0}),
	m_renderContext(window)
	{

	}

	D3DRenderer::~D3DRenderer()
	{

	}

	void D3DRenderer::Initialize()
	{

	}

	void D3DRenderer::BeginRender()
	{
	}

	void D3DRenderer::EndRender()
	{
	}

	void D3DRenderer::Close()
	{
	}
}
