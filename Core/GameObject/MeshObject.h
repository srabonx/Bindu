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

		void	SetMesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, std::uint32_t vertexCount, std::uint32_t indexCount);

		virtual void Render(const D3DCommandContext& commandContext, const D3DPipelineStateManager& pipelineManager, const std::string& pipelineName);

	private:

		VertexBuffer		m_vertexBuffer;
		IndexBuffer			m_indexBuffer;

		std::uint32_t		m_vertexCount{0};
		std::uint32_t		m_indexCount{0};

	};
}

#endif