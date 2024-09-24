#ifndef FLYFRAME_H
#define FLYFRAME_H

#include <cstdint>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>
#include "UploadBuffer.h"

namespace BINDU
{
	using namespace Microsoft::WRL;
	using namespace DirectX;

	class D3DDeviceManager;

	struct PerPassConstants
	{
		XMFLOAT4X4	ViewMatrix;

		XMFLOAT4X4	ProjMatrix;

		XMFLOAT4X4	ViewProjMatrix;

		XMFLOAT4X4	InvViewMatrix;

		XMFLOAT4X4	InvProjMatrix;

		XMFLOAT4X4	InvViewProjMatrix;

		XMFLOAT3	EyePos;		// Camera Pos

		float		TotalTime{ 0.0f };
	};

	struct FrameResource
	{
		// Fence value of this frame
		std::uint64_t					FenceValue{ 0 };

		// Constant Buffer for this frame
		std::unique_ptr<UploadBuffer>	PerPassCb;

		// Command allocator for this frame
		ComPtr<ID3D12CommandAllocator>	CommandAllocator{ nullptr };

	};

	class FlyFrame
	{
	public:
		FlyFrame(std::uint8_t count);

		~FlyFrame();

		void				Initialize(const std::shared_ptr<D3DDeviceManager>& parentDeviceManager);

		void				Update();

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

		bool								m_initialized{ false };
	};
}
#endif