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

		void			Resize();

		void			Close();

	private:

		Win32Window*								m_window{ nullptr };

		// RenderDevice
		std::shared_ptr<D3DRenderDevice>			m_renderDevice;

		// Render Texture of this Renderer
		std::shared_ptr<RenderTexture>				m_renderTexture;


		std::unique_ptr<FlyFrame>					m_flyFrame;

		D3D12_VIEWPORT								m_viewport;
		D3D12_RECT									m_rect;
	};
}

#endif