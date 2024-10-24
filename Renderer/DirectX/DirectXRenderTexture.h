#ifndef BINDU_DIRECTX_RENDER_TEXTURE_H
#define BINDU_DIRECTX_RENDER_TEXTURE_H

#include "../Texture.h"
#include <d3d12.h>
#include <wrl/client.h>

#include "DescriptorHeapManager.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	class DirectXRenderTexture : public Texture
	{
		friend class DirectXRenderer;

	public:
		DirectXRenderTexture(const TextureSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);

		void Bind(std::uint32_t slot) override;

		void BindAsRt();

		const TextureSpecification& GetSpecification() const override;

		std::uint32_t GetWidth() override;

		std::uint32_t GetHeight() override;

		bool IsLoaded() override;

		bool					  IsShaderVisible() const;
		DescriptorHeapAllocation& GetRtvAllocation();
		DescriptorHeapAllocation& GetSrvAllocation();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilViewHandle() const;


		void TransitionTo(D3D12_RESOURCE_STATES newState);

		ID3D12Resource* GetResource() const;

	private:

		ComPtr<ID3D12Resource>	m_d3dResource{ nullptr };

		ComPtr<ID3D12GraphicsCommandList> m_commandList{ nullptr };

		TextureSpecification	m_specification;

		DescriptorHeapAllocation m_rtvAllocation;

		DescriptorHeapAllocation m_srvAllocation;

		D3D12_CPU_DESCRIPTOR_HANDLE	m_dsvHandle;

		bool					m_isLoaded{ false };

		bool					m_isShaderVisible{ false };

		D3D12_RESOURCE_STATES	m_currState{ D3D12_RESOURCE_STATE_COMMON };
	};
}

#endif
