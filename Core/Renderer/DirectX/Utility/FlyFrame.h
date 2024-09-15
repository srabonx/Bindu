#ifndef FLYFRAME_H
#define FLYFRAME_H

#include <cstdint>
#include <d3d12.h>
#include <vector>
#include <wrl/client.h>
#include "UploadBuffer.h"
#include "../D3DRenderDevice.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DRenderDevice;

	struct FrameResource
	{
		// Fence value of this frame
		std::uint64_t		FenceValue{ 0 };

		// Constant Buffer for this frame
		std::unique_ptr<UploadBuffer>  ConstantBuffer;

		// Command allocator for this frame
		ComPtr<ID3D12CommandAllocator> CommandAllocator{ nullptr };
	};

	class FlyFrame
	{
	public:
		FlyFrame(std::uint8_t count);

		~FlyFrame();

		void				Initialize(const std::shared_ptr<D3DRenderDevice>& pRenderDevice);

		template<typename T>
		void				InitializeConstantBuffers(std::uint32_t elementPerFrame)
		{
			if (!m_initialized)
				THROW_EXCEPTION(3, "FlyFrame is not initialized");

			auto d3dDevice = m_parentDevice->GetD3DDevice();

			for(auto& f : m_frames)
			{
				f.ConstantBuffer = std::make_unique<UploadBuffer>(true);
				f.ConstantBuffer->Initialize<T>(d3dDevice, elementPerFrame);
			}
		}

		void				Update();

		FrameResource&		GetCurrentFrame();
	private:

		// Frame Resources
		std::vector<FrameResource>		 m_frames;

		std::shared_ptr<D3DRenderDevice> m_parentDevice{ nullptr };

		std::uint8_t					 m_frameCount{ 0 };

		std::uint8_t					 m_currentFrame{ 0 };

		bool							 m_initialized{ false };
	};
}
#endif