#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H


#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl/client.h>

namespace BINDU
{

	class RenderTexture
	{
	public:
		explicit RenderTexture(DXGI_FORMAT format);
		~RenderTexture();

		void SetDevice(ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor);

		void SizeResources(size_t width, size_t height, ID3D12GraphicsCommandList* pCommandList);

		void ReleaseDevice();

		void TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES stateAfter);

		void BeginScene(ID3D12GraphicsCommandList* commandList);

		void Clear(ID3D12GraphicsCommandList* commandList);

		void EndScene(ID3D12GraphicsCommandList* commandList);

		void SetClearColor(DirectX::FXMVECTOR color);

		Microsoft::WRL::ComPtr<ID3D12Resource>& GetResource();

		DXGI_FORMAT GetFormat() const;

		size_t			GetHeight() const;
		size_t			GetWidth() const;


	private:
		class Impl;
		std::unique_ptr<Impl> m_impl{ nullptr };

	};

}


#endif