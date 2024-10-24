#ifndef BINDU_DIRECTX_CONSTANT_BUFFER_H
#define BINDU_DIRECTX_CONSTANT_BUFFER_H

#include "../UniformBuffer.h"
#include <d3d12.h>
#include <wrl/client.h>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class DirectXConstantBuffer : public UniformBuffer
	{
		friend class D3DDeviceManager;
	public:

		DirectXConstantBuffer(std::uint32_t elementCount, std::uint32_t elementByteSize);

		~DirectXConstantBuffer() override;

		DirectXConstantBuffer(DirectXConstantBuffer&& other) noexcept;

		DirectXConstantBuffer& operator = (DirectXConstantBuffer&& other) noexcept;

		void SetData(const void* data, std::uint32_t size, std::uint32_t offset) override;

		D3D12_GPU_VIRTUAL_ADDRESS	GetGPUVirtualAddressAt(std::uint32_t index) const;

	private:

		ComPtr<ID3D12Resource>	m_resource{ nullptr };

		BYTE*					m_mappedData{ nullptr };

		std::uint32_t			m_elementByteSize{ 0 };

		std::uint32_t			m_elementCount{ 0 };
	};
}

#endif