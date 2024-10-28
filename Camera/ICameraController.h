#ifndef BINDU_ICAMERA_CONTROLLER_H
#define BINDU_ICAMERA_CONTROLLER_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;
	
	class Camera;

	class ICameraController
	{
	public:
		virtual ~ICameraController() = default;

		virtual void  UpdateViewMatrix() = 0;

		virtual void  Orbit(float deltaYaw, float deltaPitch) = 0;

		virtual const XMFLOAT3 GetPosition() const = 0;

		virtual const XMFLOAT4X4& GetViewProjection4x4() const = 0;

		virtual const XMMATRIX& GetProjection() const = 0;

		virtual const XMMATRIX& GetView() const = 0;

		virtual const XMMATRIX& GetViewProjection() const = 0;
	};
}

#endif