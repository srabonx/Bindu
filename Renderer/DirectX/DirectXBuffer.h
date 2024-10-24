#ifndef BINDU_DIRECTX_BUFFER_H
#define BINDU_DIRECTX_BUFFER_H

#include <d3d12.h>
#include <wrl/client.h>

#include "../Buffer.h"


namespace BINDU
{
	using namespace Microsoft::WRL;

	class DirectXVertexBuffer : public VertexBuffer
	{
		friend class DirectXRenderer;
	public:
		DirectXVertexBuffer(const ComPtr<ID3D12GraphicsCommandList>& commandList);

		// Inherited via VertexBuffer
		void Bind() override;

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

		void DisposeUploader();

	private:
		ComPtr<ID3D12GraphicsCommandList> m_commandList{ nullptr };

		ComPtr<ID3DBlob>			m_bufferCpu{ nullptr };

		ComPtr<ID3D12Resource>		m_bufferGpu{ nullptr };

		ComPtr<ID3D12Resource>		m_bufferUploader{ nullptr };

		std::uint32_t				m_sizeInBytes{ 0 };

		std::uint32_t				m_byteStride{ 0 };

		std::uint32_t				m_count{ 0 };

	};

	class DirectXIndexBuffer : public IndexBuffer
	{
		friend class DirectXRenderer;
	public:
		DirectXIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& commandList);

		void Bind() override;

		std::uint32_t GetCount() const override;

		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

		void DisposeUploader();

	private:
		ComPtr<ID3D12GraphicsCommandList> m_commandList{ nullptr };

		ComPtr<ID3DBlob>			m_bufferCpu{ nullptr };

		ComPtr<ID3D12Resource>		m_bufferGpu{ nullptr };

		ComPtr<ID3D12Resource>		m_bufferUploader{ nullptr };

		std::uint32_t				m_sizeInBytes{ 0 };

		std::uint32_t				m_byteStride{ 0 };

		std::uint32_t				m_count{ 0 };

		DXGI_FORMAT					m_dxgiFormat{ DXGI_FORMAT_UNKNOWN };
	};
}

#endif