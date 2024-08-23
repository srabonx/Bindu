#ifndef UPLOADBUFFER_H
#define UPLOADBUFFER_H

#include "D3DUtillity.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	template<typename T>
	class UploadBuffer
	{
	public:
		/*
		*	pDevice				=	Pointer to the Direct3D device
		*	elementCount		=	Elements to be stored in the buffer
		*	isConstantBuffer	=	Set this to true is you want this UploadBuffer to be treated as a constant buffer
		*/
		UploadBuffer(ID3D12Device* pDevice, UINT elementCount, bool isConstantBuffer = false) : m_isConstantBuffer(isConstantBuffer)
		{
			m_elementByteSize = sizeof(T);

			// if the buffer is to be a constant buffer then we need to calculate the appropiate constant buffer byteSize
			if (isConstantBuffer)
				m_elementByteSize = D3DUtility::CalculateConstantBufferByteSize(m_elementByteSize);

			// Create the Buffer with heap type Upload
			CD3DX12_HEAP_PROPERTIES	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC	rsDesc = CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize * elementCount);

			DXThrowIfFailed(pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf())));


			// Map the data of the upload buffer to be modified later on
			DXThrowIfFailed(m_resource->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedData)));

			
			// We can hold onto the mapped data until we are done with the resource,
			// but we need to make sure that we do not write to the buffer while the GPU is reading from it.
			// So CPU, GPU synchronization must be in place
			
		}

		// No copy constructor
		UploadBuffer(const UploadBuffer& rhs) = delete;
		// No assignment operator
		UploadBuffer& operator = (const UploadBuffer& rhs) = delete;

		// When we are done with the resource, release the mapped data
		~UploadBuffer()
		{
			if (m_resource != nullptr)
				m_resource->Unmap(0, nullptr);
		}

		// Returns the underlying resource of the UploadBuffer
		ID3D12Resource* Resource() const
		{
			return m_resource.Get();
		}

		void CopyData(UINT elementIndex, const T& data)
		{
			// copy the data to the memory location of element at elementIndex
			memcpy(&m_mappedData[elementIndex * m_elementByteSize], &data, sizeof(T));
		}

		// Returns if this buffer is a constant buffer or not
		bool IsConstantBuffer() const { return m_isConstantBuffer; }

		// Returns the size of elements in the buffer in bytes
		UINT GetElementByteSize() const { return m_elementByteSize; }

	private:
		// The underlying resource
		ComPtr<ID3D12Resource>		m_resource{ nullptr };

		// Internal data of the m_resource
		BYTE*						m_mappedData{ nullptr };

		// Is this buffer a constant buffer
		bool						m_isConstantBuffer{ false };

		// ByteSize of the elements in buffer
		UINT						m_elementByteSize{ 0 };
	};
}

#endif