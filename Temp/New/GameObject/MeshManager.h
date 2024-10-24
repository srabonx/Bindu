#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Buffers.h"
#include <map>
#include <string>
#include <vector>

#include "../../Utility/Common/DynamicVoidBuffer.h"

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

	class MeshManager;


	class MeshGeometry
	{
		friend class MeshManager;

	public:
		MeshGeometry(std::uint32_t vertexStructByteSize, DXGI_FORMAT indexFormat);

		~MeshGeometry() = default;

		MeshGeometry(MeshGeometry&& rhs) noexcept;

		MeshGeometry& operator = (MeshGeometry&& rhs) noexcept;

		void		AddMesh(const std::string& name, const void* vertexData, std::uint32_t vertexCount, const void* indexData, std::uint32_t indexCount,
							D3D12_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		MeshData	FindMesh(const std::string& name);

	private:
		std::string							m_name;

		std::unique_ptr<VertexBuffer>		m_vertexBuffer{ nullptr };
		std::unique_ptr<IndexBuffer>		m_indexBuffer{ nullptr };

		std::uint64_t						m_lastStartIndexOffset{ 0 };
		std::uint64_t						m_lastBaseVertexOffset{ 0 };

		std::uint32_t						m_vertexStructByteSize{ 0 };
		std::uint32_t						m_indexFormatByteSize{ 0 };

		DynamicVoidBuffer					m_intermediateVertexData;

		DynamicVoidBuffer					m_intermediateIndexData;

		std::map <std::string, MeshData>	m_meshDataPool;
	};


	/*template<class V, typename I>
	class MeshGeometry
	{
		friend class MeshManager<V,I>;

	public:

		MeshGeometry() //: m_vertexBuffer(sizeof(V)), m_indexBuffer(DXGI_FORMAT_R16_UINT)
		{
			m_vertexBuffer = std::make_unique<VertexBuffer>(sizeof(V));

			if(sizeof(I) == sizeof(std::uint32_t))
				m_indexBuffer = std::make_unique<IndexBuffer>(DXGI_FORMAT_R32_UINT);
			else if(sizeof(I) == sizeof(std::uint16_t))
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
	}; */

	class MeshManager
	{
	public:
		MeshManager() = default;
		~MeshManager() = default;

		void				AddGeometry(const D3DCommandContext& commandContext, const std::string& geometryName, MeshGeometry&& geometry);

		MeshData			GetMesh(const std::string& baseGeometryName, const std::string& meshName);

	private:

		std::map<std::string, MeshGeometry>			m_meshGeoPool;

	};
}

#endif