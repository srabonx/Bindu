#ifndef BINDU_CAMERA_CONTROLLER_H
#define BINDU_CAMERA_CONTROLLER_H

#include <DirectXMath.h>
#include "ICameraController.h"

namespace BINDU
{
	class Camera;
}

namespace BINDU
{
	using namespace DirectX;

	class CameraController : public ICameraController
	{
	public:

		CameraController() = default;

		CameraController(Camera* camera);

		~CameraController() = default;

		void SetCamera(Camera* camera);

		void SetPosition(const DirectX::XMFLOAT3& position);

		void Orbit(float deltaYaw, float deltaPitch) override;

		void UpdateViewMatrix() override;

		void StrafeX(float d);

		void StrafeY(float d);

		void Walk(float d);

		void Pitch(float angle);

		void RotateY(float angle);

		void Roll(float angle);

		const XMFLOAT3 GetPosition() const override;

		const XMFLOAT4X4& GetViewProjection4x4() const override;

		const XMMATRIX& GetProjection() const override;

		const XMMATRIX& GetView() const override;

		const XMMATRIX& GetViewProjection() const override;

	private:
		void ReticulateViewMatrix();
	private:

		Camera*		m_camera{ nullptr };

		XMFLOAT3	m_right{ 1.f,0.f,0.f };
		XMFLOAT3	m_up{ 0.f,1.0f,0.0f };
		XMFLOAT3	m_look{ 0.0f,0.0f,1.0f };

		XMFLOAT3	m_position{ 0.0f,0.0f,0.0f };

		float		m_pitch{ 0.f };
		float		m_yaw{ 0.f };
		float		m_roll{ 0.f };

	};
}

#endif