#define NOMINMAX
#include "OrthographicCamera.h"

#include <algorithm>

namespace BINDU
{
	OrthographicCamera::OrthographicCamera(std::uint16_t width, std::uint16_t height)
	{
		Resize(width, height);
	}

	void OrthographicCamera::Zoom(float zoom)
	{
		m_zoom = std::max(zoom, 0.25f);

		float viewWidth = m_zoom * m_aspectRatio;

		float left = -viewWidth;
		float right = viewWidth;
		float bottom = -m_zoom;
		float top = m_zoom;

		UpdateProjectionMatrix(left, right, bottom, top);
	}

	void OrthographicCamera::Resize(std::uint16_t viewPortWidth, std::uint16_t viewPortHeight)
	{
		m_aspectRatio = static_cast<float>(viewPortWidth) / static_cast<float>(viewPortHeight);
				
		float viewWidth = m_zoom * m_aspectRatio;

		float left = -viewWidth;// / 2.0f;
		float right = viewWidth;// 2.0f;
		float bottom = -m_zoom;//-viewHeight / 2.0f;
		float top = m_zoom;//viewHeight / 2.0f;

		UpdateProjectionMatrix(left, right, bottom, top);
	}

	void OrthographicCamera::UpdateProjectionMatrix(float left, float right, float bottom, float top)
	{
		auto orthoMat = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, -1.0f, 1.0f);
		XMStoreFloat4x4(&m_projMatrix, XMMatrixTranspose(orthoMat));
	}
}
