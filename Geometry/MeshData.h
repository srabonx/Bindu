#ifndef BINDU_MESH_DATA_H
#define BINDU_MESH_DATA_H

#include "../Renderer/Buffer.h"
#include <string>

namespace BINDU
{

	enum class PrimitiveType
	{
		POINT_LIST,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STIP		
	};

	struct MeshData
	{
		std::string			Name;

		VertexBuffer*		p_VertexBuffer{ nullptr };

		IndexBuffer*		p_IndexBuffer{ nullptr };

		PrimitiveType		Type{ PrimitiveType::TRIANGLE_LIST };

		std::uint32_t		IndexCount{ 0 };

		std::uint32_t		StartIndexLocation{ 0 };

		std::uint32_t		BaseVertexLocation{ 0 };
	};
}

#endif