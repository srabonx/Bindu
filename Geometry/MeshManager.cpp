#include "MeshManager.h"

#include "../Renderer/ResourceFactory.h"

namespace BINDU
{
	MeshGeometry::MeshGeometry(std::uint32_t vertexStructByteSize, DataFormat indexFormat) : m_vertexStructByteSize(vertexStructByteSize) ,
																								m_indexDataFormat(indexFormat),
																								m_intermediateVertexData(0),
																								m_intermediateIndexData(0)
	{
		switch (indexFormat)
		{
		case DataFormat::UINT_16:
			m_indexFormatByteSize = sizeof(std::uint16_t);
			break;
		case DataFormat::UINT_32:
			m_indexFormatByteSize = sizeof(std::uint32_t);
			break;
		default:
			break;
		}

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
		std::swap(m_indexDataFormat, rhs.m_indexDataFormat);
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
		std::swap(m_indexDataFormat, rhs.m_indexDataFormat);
		m_intermediateVertexData = std::move(rhs.m_intermediateVertexData);
		m_intermediateIndexData = std::move(rhs.m_intermediateIndexData);
		//m_meshDataPool.insert(std::make_move_iterator(rhs.m_meshDataPool.begin()), std::make_move_iterator(rhs.m_meshDataPool.end()));

		//rhs.m_meshDataPool.clear();
		m_meshDataPool = std::move(rhs.m_meshDataPool);

		return *this;
	}

	void MeshGeometry::AddMesh(const std::string& name, const void* vertexData, std::uint32_t vertexCount,
		const void* indexData, std::uint32_t indexCount, PrimitiveType primitiveTopology)
	{
		// Append the vertex and index data to the intermediate buffer
		std::uint32_t vbByteSize = vertexCount * m_vertexStructByteSize;
		std::uint32_t ibByteSize = indexCount * m_indexFormatByteSize;

		m_intermediateVertexData.Append(vertexData, vbByteSize);

		m_intermediateIndexData.Append(indexData, ibByteSize);

		// Build the mesh data for this mesh
		MeshData meshData = {};
		meshData.Name = name;
		meshData.IndexCount = indexCount;
		meshData.StartIndexLocation = static_cast<std::uint32_t>(m_lastStartIndexOffset);
		meshData.BaseVertexLocation = static_cast<std::uint32_t>(m_lastBaseVertexOffset);
		meshData.Type = primitiveTopology;

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

	void MeshManager::AddGeometry(const std::string& geometryName, MeshGeometry&& geometry)
	{
		// Create the vertex buffer
		geometry.m_vertexBuffer = ResourceFactory::CreateVertexBuffer(geometry.m_intermediateVertexData.GetBufferData(),
			static_cast<std::uint32_t>(geometry.m_lastBaseVertexOffset), geometry.m_vertexStructByteSize);

		// Create the index Buffer
		geometry.m_indexBuffer = ResourceFactory::CreateIndexBuffer(geometry.m_intermediateIndexData.GetBufferData(),
			static_cast<std::uint32_t>(geometry.m_lastStartIndexOffset), geometry.m_indexFormatByteSize, geometry.m_indexDataFormat);


		// Clear the intermediate buffer data
		geometry.m_intermediateVertexData.Free();
		geometry.m_intermediateIndexData.Free();

		// Set mesh data vertex and index buffer
		for(auto& data : geometry.m_meshDataPool)
		{
			data.second.p_VertexBuffer = geometry.m_vertexBuffer.get();
			data.second.p_IndexBuffer = geometry.m_indexBuffer.get();
		}

		geometry.m_name = geometryName;

		m_meshGeoPool.emplace(geometryName, std::move(geometry));
	}

	void MeshManager::RemoveGeometry(const std::string& name)
	{
		m_meshGeoPool.erase(name);
	}

	MeshData MeshManager::GetMesh(const std::string& baseGeometryName, const std::string& meshName)
	{
		return m_meshGeoPool.find(baseGeometryName)->second.FindMesh(meshName);
	}
}
