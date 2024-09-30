#ifndef FLYFRAME_H
#define FLYFRAME_H

#include <cstdint>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>
#include "UploadBuffer.h"
#include "../../GameObject/Light.h"

namespace BINDU
{
	using namespace Microsoft::WRL;
	using namespace DirectX;

	class D3DDeviceManager;
	class ConstantBufferManager;

	struct PerPassConstants
	{
		XMFLOAT4X4	ViewMatrix;

		XMFLOAT4X4	ProjMatrix;

		XMFLOAT4X4	ViewProjMatrix;

		XMFLOAT4X4	InvViewMatrix;

		XMFLOAT4X4	InvProjMatrix;

		XMFLOAT4X4	InvViewProjMatrix;

		XMFLOAT3	EyePos;		// Camera Pos

		float		CbPerPassPad1;

		XMFLOAT2	RenderTargetSize;

		XMFLOAT2	InvRenderTargetSize;

		float		NearZ;

		float		FarZ;

		float		TotalTime{ 0.0f };

		float		DeltaTime{ 0.0f };

		XMFLOAT4	FogColor;

		float		FogStart;

		float		FogRange;

		XMFLOAT2	CbPerPassPad2;
	};

	struct FrameResource
	{
		// Fence value of this frame
		std::uint64_t								FenceValue{ 0 };

		// Constant Buffer for this frame
		std::unique_ptr<UploadBuffer>				PerPassCb;

		std::unique_ptr<UploadBuffer>				ObjectsCb;

		std::unique_ptr<UploadBuffer>				MaterialCb;

		std::unique_ptr<UploadBuffer>				LightCb;

		// Command allocator for this frame
		ComPtr<ID3D12CommandAllocator>				CommandAllocator{ nullptr };

	};

	class FlyFrame
	{
	public:
		FlyFrame(std::uint8_t frameCount, std::uint64_t maxObjectsPerFrame);

		~FlyFrame();

		void				Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager, std::uint32_t perPassConstantDataByteSize, std::uint32_t perObjectConstantDataByteSize);

		void				Update();

		void				UpdatePerPassCB(std::uint64_t cbIndex, void const* data);

		void				UpdateObjectsCB(std::uint64_t cbIndex, void const* data);

		FrameResource*		GetCurrentFrame();

		D3DDeviceManager*	GetParentDeviceManager() const;

		std::uint16_t		GetFrameCount() const;

		FrameResource*		GetFrame(std::uint16_t index);

	private:

		// Frame Resources
		std::vector<FrameResource>			m_frames;

		std::shared_ptr<D3DDeviceManager>	m_parentDeviceManager{ nullptr };

		std::uint8_t						m_frameCount{ 0 };

		std::uint8_t						m_currentFrame{ 0 };

		std::uint64_t						m_maxObjects{ 0 };

		bool								m_initialized{ false };
	};
}
#endif