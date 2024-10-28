#ifndef BINDU_CAMERA_H
#define BINDU_CAMERA_H

#include <DirectXMath.h>
#include "../MathHelper/MathHelper.h"

namespace BINDU
{
	using namespace DirectX;

	class Camera
	{
	public:
		Camera() = default;
		
		virtual ~Camera() = default;

		virtual void Resize(std::uint16_t viewportWidth, std::uint16_t viewportHeight) = 0;

		const XMMATRIX& GetProjection() const { return XMLoadFloat4x4(&m_projMatrix); }

		const XMFLOAT4X4& GetProjection4x4() const
		{
			auto mat = XMMatrixTranspose(GetProjection());
			XMFLOAT4X4 projMat{};

			XMStoreFloat4x4(&projMat, mat);

			return projMat;
		}

	protected:
		XMFLOAT4X4		m_projMatrix = MathHelper::Identity4x4();
	};
}

#endif