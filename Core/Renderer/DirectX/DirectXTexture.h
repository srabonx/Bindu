#ifndef BINDU_DIRECTX_TEXTURE_H
#define BINDU_DIRECTX_TEXTURE_H

#include "../Texture.h"
#include <d3d12.h>
#include <wrl/client.h>

#include "DescriptorHeapManager.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	class DirectXTexture : public Texture
	{
		friend class DirectXRenderer;

	public:
		DirectXTexture(const TextureSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& commandList);

		void Bind(std::uint32_t slot) override;

		const TextureSpecification& GetSpecification() const override;

		std::uint32_t GetWidth() override;
		
		std::uint32_t GetHeight() override;
		
		bool IsLoaded() override;

	private:
		
		ComPtr<ID3D12Resource>	m_d3dResource{ nullptr };

		ComPtr<ID3D12Resource>	m_d3dUploader{ nullptr };

		ComPtr<ID3D12GraphicsCommandList> m_commandList{ nullptr };

		TextureSpecification	m_specification;

		DescriptorHeapAllocation m_allocation;

		bool					m_isLoaded{ false };
	};
}

#endif
