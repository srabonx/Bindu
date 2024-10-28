#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <string>
#include <unordered_map>

#include "MeshData.h"
#include "../Renderer/DataFormat.h"
#include "../Utility/Common/DynamicVoidBuffer.h"
#include "../Utility/Common/CommonUtility.h"

namespace BINDU
{

	class MeshManager;


	class MeshGeometry
	{
		friend class MeshManager;

	public:
		MeshGeometry(std::uint32_t vertexStructByteSize, DataFormat indexFormat);

		~MeshGeometry() = default;

		MeshGeometry(MeshGeometry&& rhs) noexcept;

		MeshGeometry& operator = (MeshGeometry&& rhs) noexcept;

		void		AddMesh(const std::string& name, const void* vertexData, std::uint32_t vertexCount, const void* indexData, std::uint32_t indexCount,
							PrimitiveType primitiveTopology = PrimitiveType::TRIANGLE_LIST);

		MeshData	FindMesh(const std::string& name);

	private:
		std::string							m_name;

		Ref<VertexBuffer>					m_vertexBuffer{ nullptr };
		Ref<IndexBuffer>					m_indexBuffer{ nullptr };

		std::uint64_t						m_lastStartIndexOffset{ 0 };
		std::uint64_t						m_lastBaseVertexOffset{ 0 };

		std::uint32_t						m_vertexStructByteSize{ 0 };
		std::uint32_t						m_indexFormatByteSize{ 0 };
		DataFormat							m_indexDataFormat;

		DynamicVoidBuffer					m_intermediateVertexData;

		DynamicVoidBuffer					m_intermediateIndexData;

		std::unordered_map<std::string, MeshData>	m_meshDataPool;
	};


	class MeshManager
	{
	public:
		MeshManager() = default;
		~MeshManager() = default;

		void				AddGeometry(const std::string& geometryName, MeshGeometry&& geometry);

		void				RemoveGeometry(const std::string& name);

		MeshData			GetMesh(const std::string& baseGeometryName, const std::string& meshName);

	private:

		std::unordered_map<std::string, MeshGeometry>			m_meshGeoPool;

	};
}

#endif