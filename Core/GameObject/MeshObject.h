#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <cstdint>
#include <string>
#include "Buffers.h"
#include "GameObject.h"

namespace BINDU
{
	class D3DCommandContext;
	class D3DPipelineStateManager;

	class MeshObject : public GameObject
	{
	public:
		MeshObject();

		MeshObject(const MeshObject& rhs) = default;

		MeshObject& operator = (const MeshObject& rhs) = default;

		void	SetMesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, std::uint32_t vertexCount, std::uint32_t indexCount);

		void	SetMesh(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, std::uint32_t vertexCount, std::uint32_t indexCount);


		virtual void Render(const D3DCommandContext& commandContext);

	private:

		VertexBuffer		m_vertexBuffer;
		IndexBuffer			m_indexBuffer;

		std::uint32_t		m_vertexCount{0};
		std::uint32_t		m_indexCount{0};

	};
}

#endif