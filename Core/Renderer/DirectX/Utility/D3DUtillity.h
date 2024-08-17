#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include "../Error/ComException.h"
#include <d3d12.h>

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

constexpr UINT SwapChainBufferCount = 2;

namespace BINDU::D3DUtility
{
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
}


#endif