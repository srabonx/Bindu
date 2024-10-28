#ifndef BINDU_ORTHOGRAPHIC_CAMERA
#define BINDU_ORTHOGRAPHIC_CAMERA

#include "Camera.h"

namespace BINDU
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(std::uint16_t width, std::uint16_t height);

		~OrthographicCamera() override = default;

		void Zoom(float deltaZoom);

		void Resize(std::uint16_t viewPortWidth, std::uint16_t viewPortHeight) override;

	private:
		void UpdateProjectionMatrix(float left, float right, float bottom, float top);

	private:
		float	m_maxZoom{ 100.f };
		float	m_zoom{ 0.25f };
		float	m_aspectRatio;
	};
}

#endif