#include "DirectXTexture.h"

namespace BINDU
{
	DirectXTexture::DirectXTexture(const TextureSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& commandList) :
								m_commandList(commandList), m_specification(specification)
	{
	}

	void DirectXTexture::Bind(std::uint32_t slot)
	{
		if (m_isLoaded)
		{
			m_commandList->SetGraphicsRootDescriptorTable(slot, m_allocation.GetGpuHandle());
		}
	}

	const TextureSpecification& DirectXTexture::GetSpecification() const
	{
		return m_specification;
	}

	std::uint32_t DirectXTexture::GetWidth()
	{
		return m_specification.Width;
	}

	std::uint32_t DirectXTexture::GetHeight()
	{
		return m_specification.Height;
	}

	bool DirectXTexture::IsLoaded()
	{
		return m_isLoaded;
	}
}