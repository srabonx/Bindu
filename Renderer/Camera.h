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

		const XMMATRIX& GetProjection() const { return XMLoadFloat4x4(&m_projMatrix); }

	protected:
		XMFLOAT4X4		m_projMatrix = MathHelper::Identity4x4();
	};
}

#endif