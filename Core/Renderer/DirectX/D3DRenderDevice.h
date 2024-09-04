#ifndef D3DRENDERDEVICE_H
#define D3DRENDERDEVICE_H

#include "IRenderDevice.h"
#include <d3d12.h>
#include <wrl/client.h>


namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DRenderDevice : public IRenderDevice
	{
	public:
		D3DRenderDevice();

		~D3DRenderDevice();

		void		Initialize() override;

		void		BeginRender() override;

		void		ClearScreen() override;

		void		EndRender() override;

	private:

		// Direct3D Device
		ComPtr<ID3D12Device>					m_d3dDevice{ nullptr };


	};
}

#endif