#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d12.h>
#include <wrl/client.h>

#include "../GameObject/Transform.h"
#include "../Renderer/DirectX/DescriptorHeapManager.h"

namespace BINDU
{
	using namespace Microsoft::WRL;
	using namespace DirectX;

	class Texture
	{
	public:
		Texture() = default;
		~Texture() = default;

		explicit Texture(const std::string& name);

		Texture(const std::string& name, const std::string& filepath);

		void						SetName(const std::string& name);
		
		std::string					GetName() const;

		std::string					GetFilepath() const;

		ComPtr<ID3D12Resource>&		GetResource();

		ComPtr<ID3D12Resource>&		GetUploadHeap();

		void						DiscardUploadHeap();

		void						SetHeapAllocation(DescriptorHeapAllocation&& allocation);

		DescriptorHeapAllocation*	GetHeapAllocation();


	private:
		std::string						m_name;

		std::string						m_filepath;

		ComPtr<ID3D12Resource>			m_resource{ nullptr };

		ComPtr<ID3D12Resource>			m_uploadHeap{ nullptr };

		DescriptorHeapAllocation		m_heapAllocation;
	};
}

#endif