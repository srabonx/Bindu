#ifndef BINDU_SCENE_CAMERA_CONTROLLER_H
#define BINDU_SCENE_CAMERA_CONTROLLER_H


#include "../MathHelper/MathHelper.h"
#include "../ECS/Entity.h"

#include "ICameraController.h"


namespace BINDU
{
	using namespace DirectX;

	class Camera;

	class SceneCameraController : public ICameraController
	{
	public:
		SceneCameraController() = default;

		SceneCameraController(const Ref<Entity>& cameraEntity);
		
		~SceneCameraController() override = default;

		void SetCameraEntity(const Ref<Entity>& cameraEntity);

		// Camera controls
		void Orbit(float deltaYaw, float deltaPitch) override;

		void Recenter();

		const XMFLOAT3 GetPosition() const override;

		const XMMATRIX& GetProjection() const override;

		const XMMATRIX& GetView() const override;

		const XMMATRIX& GetViewProjection() const override;

		const XMFLOAT4X4& GetViewProjection4x4() const override;

		void UpdateViewMatrix() override;


	private:
		WeakRef<Camera> m_camera;

		WeakRef<Entity> m_cameraEntity;


		float	m_yaw{ 0.f };

		float	m_pitch{ 0.f };

		XMFLOAT4X4	m_viewMat = MathHelper::Identity4x4();
	};
}

#endif