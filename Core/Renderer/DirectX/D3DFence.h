#ifndef D3DFENCE_H
#define D3DFENCE_H

#include <wrl/client.h>
#include <d3d12.h>
#include <cstdint>
#include <memory>

namespace BINDU
{
	using namespace Microsoft::WRL;

	// Forward declaration
	class D3DRenderDevice;

	class D3DCommandContext;

	class D3DFence
	{
		// This class is friends with D3DCommandContext class
		friend class D3DCommandContext;

	public:
		D3DFence() = default;

		~D3DFence() = default;

		void					Initialize(const std::shared_ptr<D3DRenderDevice>& parentDevice);

		std::uint64_t			Advance();

		D3DRenderDevice*		GetParentDevice() const;

		std::uint64_t			GetCurrentValue() const;

	private:
		// Shared pointer to the RenderDevice that created this Fence.
		std::shared_ptr<D3DRenderDevice>	m_parentDevice{ nullptr };
		
		// Underlying Fence object
		ComPtr<ID3D12Fence>					m_fence{ nullptr };

		// Fence value
		std::uint64_t						m_fenceValue{ 0 };
	};
}

#endif