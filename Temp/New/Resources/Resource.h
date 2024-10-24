#ifndef RESOURCE_H
#define RESOURCE_H

#include <d3d12.h>
#include <memory>
#include <wrl/client.h>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DDeviceManager;
	class D3DCommandContext;


	struct ResourceProperties
	{

		DXGI_FORMAT				ResourceFormat{ DXGI_FORMAT_UNKNOWN };

		D3D12_RESOURCE_FLAGS	ResourceFlags{ D3D12_RESOURCE_FLAG_NONE };

		D3D12_HEAP_TYPE			HeapType{ D3D12_HEAP_TYPE_DEFAULT };

		D3D12_HEAP_FLAGS		HeapFlags{ D3D12_HEAP_FLAG_NONE };

		D3D12_RESOURCE_STATES	InitialState{ D3D12_RESOURCE_STATE_COMMON };

		DXGI_SAMPLE_DESC		SampleDesc{ 1,0 };

		D3D12_CLEAR_VALUE*		OptimizedClearValue{ nullptr };

		std::uint64_t			Width{ 0 };

		std::uint64_t			Height{ 0 };
	};

	class Resource
	{
	public:
		Resource() = default;

		virtual ~Resource() {}

		virtual void			Initialize(const ResourceProperties& properties, const std::shared_ptr<D3DDeviceManager>& parentDevice);

		void					TransitionTo(const D3DCommandContext& commandContext, D3D12_RESOURCE_STATES newState);

		void					Reset();

		virtual void			Resize(std::uint16_t width, std::uint16_t height);

		D3D12_RESOURCE_STATES	GetState() const;

		D3D12_RESOURCE_FLAGS	GetFlags() const;

		D3D12_RESOURCE_DESC		GetDesc() const;

		DXGI_FORMAT				GetFormat() const;

		ComPtr<ID3D12Resource>	GetResource() const;

	protected:
		void					CreateCommitedResource(ID3D12Device* d3dDevice, std::uint16_t width, std::uint16_t height);

	protected:

		std::shared_ptr<D3DDeviceManager> m_parentDeviceManager{ nullptr };

		// Underlying resource
		ComPtr<ID3D12Resource>			m_resource{ nullptr };

		DXGI_FORMAT						m_format{ DXGI_FORMAT_UNKNOWN };

		D3D12_RESOURCE_FLAGS			m_flags{ D3D12_RESOURCE_FLAG_NONE };

		D3D12_HEAP_TYPE					m_heapType{ D3D12_HEAP_TYPE_DEFAULT };

		D3D12_HEAP_FLAGS				m_heapFlags{ D3D12_HEAP_FLAG_NONE };

		D3D12_RESOURCE_STATES			m_currState{ D3D12_RESOURCE_STATE_COMMON };

		DXGI_SAMPLE_DESC				m_sampleDesc{ 1,0 };

		std::unique_ptr<D3D12_CLEAR_VALUE>				m_optimizedClearValue{ nullptr };

		std::uint64_t					m_width{ 0 };

		std::uint64_t					m_height{ 0 };

	};
}


#endif