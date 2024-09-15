#ifndef MESHGEOMETRY_H
#define MESHGEOMETRY_H

#include <string>
#include <d3d12.h>
#include <DirectXCollision.h>
#include <wrl/client.h>
#include <map>

namespace BINDU
{
	using namespace Microsoft::WRL;
	using namespace DirectX;

	struct Vertex
	{
		XMFLOAT3	Position;
		XMFLOAT4	Color;
	};

	struct SubMeshGeometry
	{
		// Index count of this SubMesh
		UINT		IndexCount{ 0 };
		
		// Start index location for this SubMesh in the combined IndexBuffer of MeshGeometry
		UINT		StartIndexLocation{ 0 };
		
		// Base vertex location of this SubMesh in the combined VertexBuffer of MeshGeometry
		UINT		BaseVertexLocation{ 0 };

		// Bounding box the geometry defined by this SubMesh
		BoundingBox	Bounds;

	};

	struct MeshGeometry
	{
		// Name to identify the geometry
		std::string					Name;

		// Generic chunks of memory holding the vertex and index buffer for CPU use
		ComPtr<ID3DBlob>			VertexBufferCPU{ nullptr };
		ComPtr<ID3DBlob>			IndexBufferCPU{ nullptr };

		// Vertex and index buffer for the GPU
		ComPtr<ID3D12Resource>		VertexBufferGPU{ nullptr };
		ComPtr<ID3D12Resource>		IndexBufferGPU{ nullptr };

		// Intermediate uploader buffer to upload vertex/index buffer data from CPU to GPU
		ComPtr<ID3D12Resource>		VertexBufferUploader{ nullptr };
		ComPtr<ID3D12Resource>		IndexBufferUploader{ nullptr };

		// Data about the buffers
		UINT						VertexByteStride{ 0 };
		UINT						VertexBufferByteSize{ 0 };
		DXGI_FORMAT					IndexBufferFormat{ DXGI_FORMAT_R16_UINT };
		UINT						IndexBufferByteSize{ 0 };


		// A mesh geometry can store multiple SubMesh geometry in one vertex/index buffer
		std::map<std::string, SubMeshGeometry>	SubMeshMap;


		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv = {};
			vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
			vbv.SizeInBytes	= VertexBufferByteSize;
			vbv.StrideInBytes = VertexByteStride;

			return vbv;
		}

		D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
		{
			D3D12_INDEX_BUFFER_VIEW ibv = {};
			ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
			ibv.Format = IndexBufferFormat;
			ibv.SizeInBytes = IndexBufferByteSize;
			
			return ibv;
		}

		// We can free the UploadBuffer memory after the upload has been done
		void DisposeUploaders()
		{
			VertexBufferUploader.Reset();
			IndexBufferUploader.Reset();
		}

	};
}

#endif