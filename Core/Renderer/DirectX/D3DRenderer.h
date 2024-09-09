#ifndef D3DRENDERER_H
#define D3DRENDERER_H
#include "D3DRenderContext.h"
#include "D3DRenderDevice.h"
#include "Utility/RenderTexture.h"

namespace BINDU
{
	class D3DRenderer
	{
	public:
		D3DRenderer(Win32Window* window);

		~D3DRenderer();

		void			Initialize();

		void			BeginRender();

		void			EndRender();

		void			Close();

	private:

		// RenderDevice
		D3DRenderDevice			m_renderDevice;

		// Render Texture of this Renderer
		RenderTexture			m_renderTexture;

		// Render Context of this Renderer
		D3DRenderContext		m_renderContext;
	};
}

#endif