#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include "../Error/ComException.h"
#include "../../../../Utility/Common/CommonUtility.h"

#include <d3d12.h>
#include "d3dx12.h"
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <fstream>

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

}


#endif