#ifndef DEPTHSTENCILBUFFER_H
#define DEPTHSTENCILBUFFER_H

#include <d3d12.h>
#include <wrl/client.h>
#include <memory>
#include "Resource.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"


namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DDeviceManager;

	class DepthStencilBuffer : public Resource
	{
	public:
		DepthStencilBuffer(float depth, std::uint8_t stencil);

		~DepthStencilBuffer() = default;

		void			CreateDepthStencilView(DXGI_FORMAT depthStencilFormat);

		void			CreateShaderResourceView();

		void			Resize(std::uint16_t width, std::uint16_t height) override;

		DXGI_FORMAT		GetDepthStencilFormat() const;

		float			GetDepth() const;

		std::uint8_t	GetStencil() const;

		DescriptorHeapAllocation*	GetDsvAllocation();

		DescriptorHeapAllocation*	GetSrvAllocation();

	private:
		DXGI_FORMAT					m_depthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

		DescriptorHeapAllocation	m_dsvAllocation;

		DescriptorHeapAllocation	m_srvAllocation;

		float						m_depth{ 1.0f };

		std::uint8_t				m_stencil{ 0 };
	};
}

#endif