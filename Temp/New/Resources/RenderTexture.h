#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "Resource.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"

namespace BINDU
{
	class RenderTexture : public Resource
	{
	public:
		RenderTexture() = default;

		explicit RenderTexture(ComPtr<ID3D12Resource>& resource);

		~RenderTexture() = default;

		void		Initialize(const ResourceProperties& properties, const std::shared_ptr<D3DDeviceManager>& parentDevice) override;

		void		CreateRenderTargetView(DXGI_FORMAT renderTargetFormat);

		void		CreateShaderResourceView(DXGI_FORMAT srvFormat);

		void		Resize(std::uint16_t width, std::uint16_t height) override;

		void		SetClearColor(float clearColor[4]);

		float*		GetClearColor();

		DescriptorHeapAllocation* GetRtvAllocation();

		DescriptorHeapAllocation* GetSrvAllocation();

	private:
		DXGI_FORMAT					m_renderTargetFormat{ DXGI_FORMAT_B8G8R8A8_UNORM };

		DXGI_FORMAT					m_srvFormat{ DXGI_FORMAT_B8G8R8A8_UNORM };

		DescriptorHeapAllocation	m_rtvAllocation;

		DescriptorHeapAllocation	m_srvAllocation;

		float						m_clearColor[4];

		bool						m_hasExternalResource{ false };

		bool						m_hasSrv{ false };
	};
}

#endif