#include "Texture.h"

namespace BINDU
{
	Texture::Texture(const std::string& name) : m_name(name)
	{
	}

	Texture::Texture(const std::string& name, const std::string& filepath) : m_name(name), m_filepath(filepath)
	{
	}

	void Texture::SetName(const std::string& name)
	{
		m_name = name;
	}

	std::string Texture::GetName() const
	{
		return m_name;
	}

	std::string Texture::GetFilepath() const
	{
		return m_filepath;
	}

	ComPtr<ID3D12Resource>& Texture::GetResource()
	{
		return m_resource;
	}

	ComPtr<ID3D12Resource>& Texture::GetUploadHeap()
	{
		return m_uploadHeap;
	}

	void Texture::DiscardUploadHeap()
	{
		m_uploadHeap.Reset();
	}

	void Texture::SetHeapAllocation(DescriptorHeapAllocation&& allocation)
	{
		m_heapAllocation = std::move(allocation);
	}

	DescriptorHeapAllocation* Texture::GetHeapAllocation()
	{
		return &m_heapAllocation;
	}
}
