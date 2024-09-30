#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Buffers.h"
#include <map>
#include <string>
#include <vector>

namespace BINDU
{

	struct MeshData
	{
		std::string					Name;

		VertexBuffer*				p_VertexBuffer{ nullptr };
		IndexBuffer*				p_IndexBuffer{ nullptr };

		D3D12_PRIMITIVE_TOPOLOGY	PrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

		std::uint64_t				IndexCount{ 0 };
		std::uint64_t				StartIndexLocation{ 0 };
		std::uint64_t				BaseVertexLocation{ 0 };
	};

	template<class V, typename I>
	class MeshManager;

	template<class V, typename I>
	class MeshGeometry
	{
		friend class MeshManager<V,I>;

	public:

		MeshGeometry() //: m_vertexBuffer(sizeof(V)), m_indexBuffer(DXGI_FORMAT_R16_UINT)
		{
			m_vertexBuffer = std::make_unique<VertexBuffer>(sizeof(V));
			m_indexBuffer = std::make_unique<IndexBuffer>(DXGI_FORMAT_R16_UINT);
		}

		void				AddMesh(const std::string& name, const std::vector<V>& vertexData, const std::vector<I>& indexData,
									D3D12_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			m_intermediateVertexBuffer.insert(m_intermediateVertexBuffer.end(), vertexData.begin(), vertexData.end());
			m_intermediateIndexBuffer.insert(m_intermediateIndexBuffer.end(), indexData.begin(), indexData.end());


			MeshData meshData;

			meshData.Name = name;
			meshData.p_VertexBuffer = m_vertexBuffer.get();
			meshData.p_IndexBuffer = m_indexBuffer.get();
			meshData.PrimitiveTopology = primitiveTopology;
			meshData.IndexCount = indexData.size();
			meshData.StartIndexLocation = m_lastStartIndexOffset;
			meshData.BaseVertexLocation = m_lastBaseVertexOffset;

			m_meshDataPool.emplace(name, meshData);

			m_lastBaseVertexOffset += vertexData.size();
			m_lastStartIndexOffset += indexData.size();

		}

		MeshData			Find(const std::string& name)
		{
			auto itr = m_meshDataPool.find(name);

			if (itr == m_meshDataPool.end())
				return MeshData();

			return itr->second;
		}

	private:
		std::string			m_name;

		std::unique_ptr<VertexBuffer>		m_vertexBuffer{ nullptr };
		std::unique_ptr<IndexBuffer>		m_indexBuffer{ nullptr };

		std::vector<V>		m_intermediateVertexBuffer;
		std::vector<I>		m_intermediateIndexBuffer;

		std::uint64_t		m_lastStartIndexOffset{ 0 };
		std::uint64_t		m_lastBaseVertexOffset{ 0 };

		std::map <std::string, MeshData>			m_meshDataPool;
	};

	template<class V, typename I>
	class MeshManager
	{
	public:
		MeshManager() = default;
		~MeshManager() = default;

		void				AddGeometry(const D3DCommandContext& commandContext, const std::string& geometryName, MeshGeometry<V,I>&& geometry)
		{
			auto vbByteSize = geometry.m_intermediateVertexBuffer.size() * sizeof(V);
			geometry.m_vertexBuffer->Create(commandContext, geometry.m_intermediateVertexBuffer.data(), vbByteSize);

			auto ibByteSize = geometry.m_intermediateIndexBuffer.size() * sizeof(I);
			geometry.m_indexBuffer->Create(commandContext, geometry.m_intermediateIndexBuffer.data(), ibByteSize);

			geometry.m_name = geometryName;

			geometry.m_intermediateVertexBuffer.clear();
			geometry.m_intermediateIndexBuffer.clear();

			m_meshPool.emplace(geometryName, std::move(geometry));
		}

		MeshData			GetMesh(const std::string& baseGeometryName, const std::string& meshName)
		{
			auto itr = m_meshPool.find(baseGeometryName);

			if (itr == m_meshPool.end())
				return MeshData();

			auto& geometry = itr->second;

			return geometry.Find(meshName);
		}

	private:

		std::map<std::string, MeshGeometry<V,I>>			m_meshPool;

	};
}

#endif