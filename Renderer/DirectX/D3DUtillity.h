#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include <array>

#include "ComException.h"
#include "../../Utility/Common/CommonUtility.h"

#include <d3d12.h>
#include "d3dx12.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <fstream>
#include <sstream>

#include "../DataFormat.h"
#include "../Texture.h"
#include "../../Geometry/MeshData.h"

// Macro to throw DirectX HRESULT exceptions 
#ifndef DXThrowIfFailed
#define DXThrowIfFailed(x)                                              \
{                                                                       \
    HRESULT hr__ = (x);                                                 \
    if(FAILED(hr__)) { throw ComException(__FILE__, __LINE__); }         \
}
#endif

#ifndef  DXSafe_Release
#define  DXSafe_Release(p) { if(p) { p->Release(); p = nullptr; } }
#endif

constexpr UINT SWAP_CHAIN_BUFFER_COUNT = 2;

constexpr UINT GPU_DESCRIPTOR_HEAP_DESCRIPTORS_COUNT = 1000;

namespace BINDU::D3DUtility
{
	using namespace Microsoft::WRL;

	inline HRESULT CreateDescriptorHeap(ID3D12Device* pD3dDevice, UINT descriptorCount, D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap** ppDescriptorHeap, D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.NodeMask = 0;
		heapDesc.Flags = heapFlags;
		heapDesc.NumDescriptors = descriptorCount;
		heapDesc.Type = type;

		HRESULT hr = S_OK;

		hr = pD3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(ppDescriptorHeap));

		return hr;
	}


	// Create a default buffer; A default buffer is like an array of data elements for GPU
	/*
	*	pDevice			= Pointer to the Direct3D device
	*	pCommandList	= Pointer to the Graphics command list of the device
	*	initData		= In memory data that will be used to initialize the default buffer with.
	*	byteSize		= Total size of the data passed in the initData parameter in bytes.
	*	uploadBuffer	= Intermediate buffer that will be used to copy the data from the system memory to GPU memory, 
	*					  it needs to kept alive until the copying is done.	* 
	*/
	inline ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList,
		const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer)
	{

		ComPtr<ID3D12Resource>	pDefaultBuffer{ nullptr };

		CD3DX12_HEAP_PROPERTIES	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC	rsDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

		// Create the default buffer
		DXThrowIfFailed(pDevice->CreateCommittedResource(&heapProp,
			D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_COMMON,
			nullptr, IID_PPV_ARGS(pDefaultBuffer.ReleaseAndGetAddressOf())));

		// Create the intermediate upload buffer
		heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		DXThrowIfFailed(pDevice->CreateCommittedResource(&heapProp,
			D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


		// Initialize the sub resource data
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = byteSize;
		subResourceData.SlicePitch = byteSize;

		// Transition and ready the pDefaultBuffer to copy data into
		CD3DX12_RESOURCE_BARRIER copyDest = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST);
		pCommandList->ResourceBarrier(1, &copyDest);

		// upload the subResource data into the intermediate uploadBuffer then copy it to the pDefaultBuffer
		UpdateSubresources<1>(pCommandList, pDefaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

		// Transition the pDefaultBuffer back to a GENERIC_READ state
		CD3DX12_RESOURCE_BARRIER genericRead = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ);
		pCommandList->ResourceBarrier(1, &genericRead);

		// Note: Upload buffer needs to be kept alive until the copying has been done
		return pDefaultBuffer;
	}

	// Calculates the size of constant buffers; Constant buffers needs to be a multiple of minimum hardware allocation size of 256 bytes.
	// This function pads any byteSize to the appropiate multiple of 256 bytes
	inline UINT CalculateConstantBufferByteSize(UINT byteSize)
	{
		// Add the byteSize to 255 then mask off the lower two byte which stores all bits < 256
		return (byteSize + 255) & ~255;
	}


	// Compiles shaders at runtime
	/*
	*	filename	= Filepath to a .hlsl file containing the shader program to be compiled
	*	defines		= Specify nullptr
	*	entryPoint	= The function name of the shaders entry point
	*	target		= String specifying shader program type and version
	*/
	inline ComPtr<ID3DBlob> CompileShader(const std::string& filename, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target)
	{
		// Use debug flag in debug mode
		UINT compileFlag = 0;
#if defined(DEBUG) || defined (_DEBUG)
		compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> byteCode{ nullptr };
		ComPtr<ID3DBlob> error{ nullptr };

		std::wstring file = StringToWstring(filename);

		hr = D3DCompileFromFile(
			file.c_str(),
			defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(),
			target.c_str(),
			compileFlag, 0,
			byteCode.ReleaseAndGetAddressOf(),
			error.ReleaseAndGetAddressOf()
		);

		// Output error if any
		if (error)
			THROW_EXCEPTION(3, (char*)error->GetBufferPointer());

		DXThrowIfFailed(hr);

		return byteCode;
	}

	// Loads compiled shader file (.cso)
	inline ComPtr<ID3DBlob> LoadCompiledShader(const std::string& filename)
	{
		std::ifstream fin(filename, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		std::ifstream::pos_type size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);

		ComPtr<ID3DBlob> byteCode{ nullptr };

		DXThrowIfFailed(D3DCreateBlob(size, byteCode.ReleaseAndGetAddressOf()));

		fin.read((char*)byteCode->GetBufferPointer(), size);
		fin.close();

		return byteCode;
	}

	inline std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers()
	{
		const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
				0, // shaderRegister
				D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
				D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
		const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
			1, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
		const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
			2, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
		const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
			3, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
		const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
			4, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressW
			0.0f, // mipLODBias
			8); // maxAnisotropy
		const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
			5, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressW
			0.0f, // mipLODBias
			8); // maxAnisotropy
		return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
	}


	inline void PrintXMVECTOR(const DirectX::XMVECTOR& vec)
	{
		// Convert XMVECTOR to XMFLOAT4 for easier access
		DirectX::XMFLOAT4 float4;
		DirectX::XMStoreFloat4(&float4, vec);

		std::stringstream str;

		// Print the components
		str<< "XMVECTOR: ("
			<< float4.x << ", "
			<< float4.y << ", "
			<< float4.z << ", "
			<< float4.w << ")" << std::endl;

		OutputDebugString(str.str().c_str());
	}

	inline DXGI_FORMAT GetDXGIFormatFromTextureFormat(const TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::TYPELESS:
			return DXGI_FORMAT_R24G8_TYPELESS;
		case TextureFormat::RGBA8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case TextureFormat::BGRA8_UNORM:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case TextureFormat::D24_UNORM_S8_UINT:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	inline DXGI_FORMAT GetDXGIFormatFromDataFormat(const DataFormat format)
	{
		switch(format)
		{
		case DataFormat::FLOAT_32:		return DXGI_FORMAT_R32_FLOAT;
		case DataFormat::FLOAT2_32:		return DXGI_FORMAT_R32G32_FLOAT;
		case DataFormat::FLOAT3_32:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case DataFormat::FLOAT4_32:		return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case DataFormat::INT_16:		return DXGI_FORMAT_R16_SINT;
		case DataFormat::INT2_16:		return DXGI_FORMAT_R16G16_SINT;
		case DataFormat::INT4_16:		return DXGI_FORMAT_R16G16B16A16_SINT;

		case DataFormat::INT_32:		return DXGI_FORMAT_R32_SINT;
		case DataFormat::INT2_32:		return DXGI_FORMAT_R32G32_SINT;
		case DataFormat::INT3_32:		return DXGI_FORMAT_R32G32B32_SINT;
		case DataFormat::INT4_32:		return DXGI_FORMAT_R32G32B32A32_SINT;
			
		case DataFormat::UINT_16:		return DXGI_FORMAT_R16_UINT;
		case DataFormat::UINT2_16:		return DXGI_FORMAT_R16G16_UINT;
		case DataFormat::UINT4_16:		return DXGI_FORMAT_R16G16B16A16_UINT;

		case DataFormat::UINT_32:		return DXGI_FORMAT_R32_UINT;
		case DataFormat::UINT2_32:		return DXGI_FORMAT_R32G32_UINT;
		case DataFormat::UINT3_32:		return DXGI_FORMAT_R32G32B32_UINT;
		case DataFormat::UINT4_32:		return DXGI_FORMAT_R32G32B32A32_UINT;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	inline D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopologyFromPrimitiveType(PrimitiveType type)
	{
		switch(type)
		{
		case PrimitiveType::POINT_LIST:		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		case PrimitiveType::LINE_LIST:		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		case PrimitiveType::LINE_STRIP:		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case PrimitiveType::TRIANGLE_LIST:	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case PrimitiveType::TRIANGLE_STIP:	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}

		return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

}


#endif