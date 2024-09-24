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
		std::unique_ptr<UploadBuffer>  ObjectCb;

		std::unique_ptr<UploadBuffer>  PassCb;

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
		void				InitializePerObjectConstantBuffers(std::uint32_t elementPerFrame)
		{
			if (!m_initialized)
				THROW_EXCEPTION(3, "FlyFrame is not initialized");

			auto d3dDevice = m_parentDevice->GetD3DDevice();

			for (auto& f : m_frames)
			{
				f.ObjectCb = std::make_unique<UploadBuffer>(true);
				f.ObjectCb->Initialize<T>(d3dDevice, elementPerFrame);
			}
		}

		template<typename T>
		void				InitializePerPassConstantBuffers(std::uint32_t passPerFrame)
		{
			if (!m_initialized)
				THROW_EXCEPTION(3, "FlyFrame is not initialized");

			auto d3dDevice = m_parentDevice->GetD3DDevice();

			for (auto& f : m_frames)
			{
				f.PassCb = std::make_unique<UploadBuffer>(true);
				f.PassCb->Initialize<T>(d3dDevice, passPerFrame);

				auto byteSize = f.PassCb->GetElementByteSize();

				for (int i = 0; i < passPerFrame; i++)
				{
					D3D12_GPU_VIRTUAL_ADDRESS cbAddress = f.PassCb->GetGPUVirtualAddress();

					cbAddress += i * byteSize;

					D3D12_CONSTANT_BUFFER_VIEW_DESC cbv;

					cbv.BufferLocation = cbAddress;
					cbv.SizeInBytes = byteSize;

					d3dDevice->CreateConstantBufferView(&cbv, m_passCBV.GetCpuHandle(i));
				}
			}
		}



		void				Update();

		FrameResource& GetCurrentFrame();

		D3DRenderDevice* GetParentDevice() const;

		std::uint16_t		GetFrameCount() const;

		FrameResource& GetFrame(std::uint16_t index);

		DescriptorHeapAllocation* GetGPUHeapAllocation();
	private:

		// Frame Resources
		std::vector<FrameResource>		 m_frames;

		std::shared_ptr<D3DRenderDevice> m_parentDevice{ nullptr };

		std::uint8_t					 m_frameCount{ 0 };

		std::uint8_t					 m_currentFrame{ 0 };

		bool							 m_initialized{ false };

		DescriptorHeapAllocation		 m_passCBV;
	};
}
#endif