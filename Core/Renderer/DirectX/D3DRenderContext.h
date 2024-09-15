#ifndef D3DRENDERCONTEXT_H
#define D3DRENDERCONTEXT_H

#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>
#include <wrl/client.h>
#include <vector>

#include "../../../Window/Win32Window.h"
#include "Utility/FlyFrame.h"
#include "Utility/RenderTexture.h"

namespace BINDU
{

	class D3DRenderDevice;

	using namespace Microsoft::WRL;

	class D3DRenderContext
	{
	public:
		explicit D3DRenderContext(Win32Window* window);

		~D3DRenderContext();

		void				Initialize();

		void				BeginRender(D3DRenderDevice* pRenderDevice);

		void				EndRender(D3DRenderDevice* pRenderDevice);

		void				CreateSwapChain(D3DRenderDevice* pRenderDevice, RenderTexture* pRenderTexture);

		void				AddFlyFrame(std::unique_ptr<FlyFrame>& flyFrame);

		void				Resize();

		// Returns the indexed adapter from the available adapter list
		IDXGIAdapter*		GetAdapter(int index) const;

		// Returns the WARP adapter
		IDXGIAdapter*		GetWarpAdapter();

		void				SetMSAADesc(DXGI_SAMPLE_DESC sampleDesc);


	private:
		void				InitDXGI();

		void				EnumAdapters(IDXGIFactory* pdxgiFactory);

		void				CreateSwapChain(IDXGIFactory* pdxgiFactory, D3DRenderDevice* renderDevice, RenderTexture* renderTexture);

	private:

		// Pointer to the window this render context is associated with
		Win32Window*						m_window{ nullptr };

		// Pointer to the render device that created this context
		D3DRenderDevice*					m_renderDevice{ nullptr };

		// Pointer to the render texture this context will use
		RenderTexture*						m_renderTexture{ nullptr };

		// DXGI Factory 4
		ComPtr<IDXGIFactory4>				m_dxgiFactory{ nullptr };

		// DXGI SwapChain
		ComPtr<IDXGISwapChain1>				m_dxgiSwapChain{ nullptr };

		// All the adapters (GPU) the system has
		std::vector<ComPtr<IDXGIAdapter>>	m_adapters{ nullptr };

		// WARP adapter
		ComPtr<IDXGIAdapter>				m_warpAdapter{ nullptr };

		std::unique_ptr<FlyFrame>			m_flyFrame{ nullptr };
	};
}

#endif