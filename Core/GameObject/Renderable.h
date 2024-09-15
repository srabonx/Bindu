#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <cstdint>
#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>
#include <wrl/client.h>

#include "Buffers.h"

namespace BINDU
{
	class FlyFrame;
}

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DCommandContext;
	class DescriptorHeapAllocation;

	struct Vertex;

	class Renderable
	{
	public:

		Renderable(std::uint32_t indexCount, std::uint32_t startIndexLocation, int baseVertexLocation, FlyFrame* parentFrame);

		virtual		~Renderable() {}

		virtual void Update();

		virtual void Draw(const D3DCommandContext& commandContext, std::uint32_t shaderRegister);

		void		SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		void		SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

	protected:

        std::string						m_name;

		std::unique_ptr<DescriptorHeapAllocation>		m_descriptorHeapAllocation;

		FlyFrame*						m_parentFlyFrame{ nullptr };

		ComPtr<ID3D12PipelineState>		m_pso{ nullptr };

		D3D12_PRIMITIVE_TOPOLOGY		m_primitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

		std::shared_ptr<VertexBuffer>	m_vertexBuffer;

		std::shared_ptr<IndexBuffer>	m_indexBuffer;

		std::uint32_t					m_indexCount;

		std::uint32_t					m_startIndexLocation;

		int								m_baseVertexLocation;

		DirectX::XMFLOAT4X4				m_worldMatrix;
	};
}

#endif