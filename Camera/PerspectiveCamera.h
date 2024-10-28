#ifndef BINDU_PERSPECTIVE_CAMERA_H
#define BINDU_PERSPECTIVE_CAMERA_H

#include "Camera.h"

namespace BINDU
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fovY, float aspectRatio, float nearPlane, float farPlane);
		~PerspectiveCamera() override = default;

		void SetLens(float newFovY, float newAspectRatio, float newNearPlane, float newFarPlane);

		void ChangeFovY(float newFovY);

		void Resize(std::uint16_t viewportWidth, std::uint16_t viewportHeight) override;

	private:
		void ReticulateProjectionMatrix();

	private:
		float m_fovY{ 0.f };
		float m_aspectRatio{ 0.f };
		float m_nearPlane{ 0.f };
		float m_farPlane{ 0.f };
	};
}

#endif