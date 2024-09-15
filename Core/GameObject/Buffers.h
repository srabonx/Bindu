#ifndef BUFFERS_H
#define BUFFERS_H

#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DCommandContext;

	class Buffer
	{
	public:
		virtual ~Buffer();

		virtual void Create(const D3DCommandContext& commandContext, const void* initData, std::uint64_t byteSize);

		void	DisposeUploader();

	protected:
		ComPtr<ID3DBlob>				m_bufferCpu{ nullptr };

		ComPtr<ID3D12Resource>			m_bufferGpu{ nullptr };

		ComPtr<ID3D12Resource>			m_bufferUploader{ nullptr };
	};

	class IndexBuffer : public Buffer
	{
	public:
		explicit IndexBuffer(const DXGI_FORMAT& dataFormat);


		D3D12_INDEX_BUFFER_VIEW	GetView() const;

	private:

		DXGI_FORMAT						m_dataFormat{ DXGI_FORMAT_R16_UINT };
	};

	class VertexBuffer : public Buffer
	{
	public:
		// strideInBytes = size of the Vertex structure
		explicit VertexBuffer(std::uint16_t strideInBytes);


		D3D12_VERTEX_BUFFER_VIEW	GetView() const;

	private:
		std::uint16_t				m_byteStride{ 0 };
	};
}

#endif