#include "PerspectiveCamera.h"

namespace BINDU
{
	PerspectiveCamera::PerspectiveCamera(float fovY, float aspectRatio, float nearPlane, float farPlane) :
		m_fovY(fovY), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane)
	{
		ReticulateProjectionMatrix();
	}

	void PerspectiveCamera::SetLens(float newFovY, float newAspectRatio, float newNearPlane, float newFarPlane)
	{
		m_fovY = newFovY;
		m_aspectRatio = newAspectRatio;
		m_nearPlane = newNearPlane;
		m_farPlane = newFarPlane;

		ReticulateProjectionMatrix();
	}

	void PerspectiveCamera::ChangeFovY(float newFovY)
	{
		m_fovY = newFovY;

		ReticulateProjectionMatrix();
	}

	void PerspectiveCamera::Resize(std::uint16_t viewportWidth, std::uint16_t viewportHeight)
	{
		m_aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);

		ReticulateProjectionMatrix();
	}

	void PerspectiveCamera::ReticulateProjectionMatrix()
	{
		XMStoreFloat4x4(&m_projMatrix, XMMatrixPerspectiveFovLH(m_fovY, m_aspectRatio, m_nearPlane, m_farPlane));
	}
}