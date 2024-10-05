#include "MeshObject.h"
#include "MeshManager.h"

namespace BINDU
{
	MeshGeometry::MeshGeometry(std::uint32_t vertexStructByteSize, DXGI_FORMAT indexFormat) : m_vertexStructByteSize(vertexStructByteSize) ,
																								m_intermediateVertexData(0),
																								m_intermediateIndexData(0)
	{
		switch (indexFormat)
		{
		case DXGI_FORMAT_R16_UINT:
			m_indexFormatByteSize = sizeof(std::uint16_t);
			break;
		case DXGI_FORMAT_R32_UINT:
			m_indexFormatByteSize = sizeof(std::uint32_t);
			break;
		default:
			break;
		}

		m_vertexBuffer = std::make_unique<VertexBuffer>(vertexStructByteSize);

		m_indexBuffer = std::make_unique<IndexBuffer>(indexFormat);

	}

	MeshGeometry::MeshGeometry(MeshGeometry&& rhs) noexcept : m_intermediateVertexData(0), m_intermediateIndexData(0)
	{

		std::swap(m_name, rhs.m_name);
		std::swap(m_vertexBuffer, rhs.m_vertexBuffer);
		std::swap(m_indexBuffer, rhs.m_indexBuffer);
		std::swap(m_lastStartIndexOffset, rhs.m_lastStartIndexOffset);
		std::swap(m_lastBaseVertexOffset, rhs.m_lastBaseVertexOffset);
		std::swap(m_vertexStructByteSize, rhs.m_vertexStructByteSize);
		std::swap(m_indexFormatByteSize, rhs.m_indexFormatByteSize);
		m_intermediateVertexData = std::move(rhs.m_intermediateVertexData);
		m_intermediateIndexData = std::move(rhs.m_intermediateIndexData);
		//m_meshDataPool.insert(std::make_move_iterator(rhs.m_meshDataPool.begin()), std::make_move_iterator(rhs.m_meshDataPool.end()));

		//rhs.m_meshDataPool.clear();

		m_meshDataPool = std::move(rhs.m_meshDataPool);

	}

	MeshGeometry& MeshGeometry::operator=(MeshGeometry&& rhs) noexcept
	{
		std::swap(m_name, rhs.m_name);
		std::swap(m_vertexBuffer, rhs.m_vertexBuffer);
		std::swap(m_indexBuffer, rhs.m_indexBuffer);
		std::swap(m_lastStartIndexOffset, rhs.m_lastStartIndexOffset);
		std::swap(m_lastBaseVertexOffset, rhs.m_lastBaseVertexOffset);
		std::swap(m_vertexStructByteSize, rhs.m_vertexStructByteSize);
		std::swap(m_indexFormatByteSize, rhs.m_indexFormatByteSize);
		m_intermediateVertexData = std::move(rhs.m_intermediateVertexData);
		m_intermediateIndexData = std::move(rhs.m_intermediateIndexData);
		//m_meshDataPool.insert(std::make_move_iterator(rhs.m_meshDataPool.begin()), std::make_move_iterator(rhs.m_meshDataPool.end()));

		//rhs.m_meshDataPool.clear();
		m_meshDataPool = std::move(rhs.m_meshDataPool);

		return *this;
	}

	void MeshGeometry::AddMesh(const std::string& name, const void* vertexData, std::uint32_t vertexCount,
	                           const void* indexData, std::uint32_t indexCount, D3D12_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		// Append the vertex and index data to the intermediate buffer
		std::uint32_t vbByteSize = vertexCount * m_vertexStructByteSize;
		std::uint32_t ibByteSize = indexCount * m_indexFormatByteSize;

		m_intermediateVertexData.Append(vertexData, vbByteSize);

		m_intermediateIndexData.Append(indexData, ibByteSize);

		// Build the mesh data for this mesh
		MeshData meshData = {};
		meshData.Name = name;
		meshData.p_VertexBuffer = m_vertexBuffer.get();
		meshData.p_IndexBuffer = m_indexBuffer.get();
		meshData.IndexCount = indexCount;
		meshData.StartIndexLocation = m_lastStartIndexOffset;
		meshData.BaseVertexLocation = m_lastBaseVertexOffset;
		meshData.PrimitiveTopology = primitiveTopology;

		// Calculate new start index and base vertex offset for the next mesh in the geometry
		m_lastStartIndexOffset += indexCount;
		m_lastBaseVertexOffset += vertexCount;

		// Add mesh to the mesh pool
		m_meshDataPool.emplace(name, meshData);
	}

	MeshData MeshGeometry::FindMesh(const std::string& name)
	{
		auto itr = m_meshDataPool.find(name);

		if (itr == m_meshDataPool.end())
			return MeshData();

		return itr->second;
	}





	// MeshGeometry Implementation

	void MeshManager::AddGeometry(const D3DCommandContext& commandContext, const std::string& geometryName,
		MeshGeometry&& geometry)
	{
		// Create the vertex buffer
		geometry.m_vertexBuffer->Create(commandContext, geometry.m_intermediateVertexData.GetBufferData(),
										geometry.m_intermediateVertexData.GetBufferSize());

		// Create the index Buffer
		geometry.m_indexBuffer->Create(commandContext, geometry.m_intermediateIndexData.GetBufferData(),
			geometry.m_intermediateIndexData.GetBufferSize());


		// Clear the intermediate buffer data's
		geometry.m_intermediateVertexData.Free();
		geometry.m_intermediateIndexData.Free();

		geometry.m_name = geometryName;

		m_meshGeoPool.emplace(geometryName, std::move(geometry));
	}

	MeshData MeshManager::GetMesh(const std::string& baseGeometryName, const std::string& meshName)
	{
		return m_meshGeoPool.find(baseGeometryName)->second.FindMesh(meshName);
	}
}
