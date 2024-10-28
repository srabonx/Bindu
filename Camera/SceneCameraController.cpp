#define NOMINMAX
#include "SceneCameraController.h"

#include "Camera.h"
#include "../Debug/Assert.h"
#include "../ECS/Components.h"

namespace BINDU
{
	SceneCameraController::SceneCameraController(const Ref<Entity>& cameraEntity) : m_cameraEntity(cameraEntity)
	{
	}

	void SceneCameraController::SetCameraEntity(const Ref<Entity>& cameraEntity)
	{
		BINDU_CORE_ASSERT(cameraEntity, "Invalid Camera Entity");

		BINDU_CORE_ASSERT(cameraEntity->HasComponent<CameraComponent>(), "Invalid Camera Entity");

		m_cameraEntity = cameraEntity;

		m_camera = m_cameraEntity.lock()->GetComponent<CameraComponent>().Camera.lock();
	}

	void SceneCameraController::Orbit(float deltaYaw, float deltaPitch)
	{
		m_yaw += deltaYaw;

		m_pitch = std::clamp(m_pitch + deltaPitch, -XM_PIDIV2, XM_PIDIV2);	// Clamping pitch to avoid flipping
	}

	void SceneCameraController::Recenter()
	{
		m_yaw = 0.0f;
		m_pitch = 0.0f;
	}

	const XMFLOAT3 SceneCameraController::GetPosition() const
	{
		if(auto cam = m_cameraEntity.lock())
		{
			auto& transformComp = cam->GetComponent<TransformComponent>();

			return transformComp.Position;
		}

		return XMFLOAT3();
	}

	const XMMATRIX& SceneCameraController::GetProjection() const
	{
		BINDU_CORE_ASSERT(m_camera, "Invalid camera set!");

		if(auto camera = m_camera.lock())
		{
			return camera->GetProjection();
		}

		return {};
	}

	const XMMATRIX& SceneCameraController::GetView() const
	{
		return XMLoadFloat4x4(&m_viewMat);
	}

	const XMMATRIX& SceneCameraController::GetViewProjection() const
	{
		auto proj = GetProjection();
		auto view = XMLoadFloat4x4(&m_viewMat);

		return XMMatrixMultiply(view, proj);
	}

	const XMFLOAT4X4& SceneCameraController::GetViewProjection4x4() const
	{
		auto viewProj = GetViewProjection();
		XMFLOAT4X4 shaderViewProj{};
		XMStoreFloat4x4(&shaderViewProj, XMMatrixTranspose(viewProj));

		return shaderViewProj;
	}

	void SceneCameraController::UpdateViewMatrix()
	{
		if (auto camEntity = m_cameraEntity.lock())
		{
			auto& transformComponent = camEntity->GetComponent<TransformComponent>();
			auto& cameraComponent = camEntity->GetComponent<CameraComponent>();

			if (cameraComponent.Primary)
			{
				auto targetEntity = camEntity->GetParentScene()->GetEntity(cameraComponent.Target);

				auto& targetEntityPos = targetEntity.GetComponent<TransformComponent>().Position;

				transformComponent.Rotation.x = m_pitch;
				transformComponent.Rotation.y = m_yaw;

				float offsetX = cameraComponent.Distance * cosf(m_pitch) * sinf(m_yaw);

				float offsetY = cameraComponent.Distance * sinf(m_pitch);

				float offsetZ = cameraComponent.Distance * cosf(m_pitch) * cosf(m_yaw);

				auto targetPos = XMLoadFloat3(&targetEntityPos);

				auto targetOffset = XMVectorSet(offsetX, offsetY + cameraComponent.HeightOffset, offsetZ, 0.0f);

				auto targetCameraPos = XMVectorAdd(targetPos, targetOffset);

				auto currentCamPos = XMLoadFloat3(&transformComponent.Position);

				if (XMVector3Equal(targetCameraPos, currentCamPos))
					return;

				auto interpolatedPos = XMVectorLerp(currentCamPos, targetCameraPos, cameraComponent.SmoothingFactor);

				XMStoreFloat3(&transformComponent.Position, interpolatedPos);

				auto viewMat = XMMatrixLookAtLH(interpolatedPos, targetPos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

				XMStoreFloat4x4(&m_viewMat, viewMat);
			}
		}
	}

}
