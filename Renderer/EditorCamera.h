#ifndef BINDU_EDITOR_CAMERA_H
#define BINDU_EDITOR_CAMERA_H

#include "Camera.h"
#include "../Event/EventStruct.h"

namespace BINDU
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		~EditorCamera() override = default;

		void OnUpdate();

		// Get Camera basis vectors
		XMVECTOR	GetRight() const;
		XMFLOAT3	GetRight3f() const;
		XMVECTOR	GetUp() const;
		XMFLOAT3	GetUp3d() const;
		XMVECTOR	GetLook() const;
		XMFLOAT3	GetLook3f() const;

		// Get frustum properties
		float		GetNearZ() const;
		float		GetFarZ() const;
		float		GetAspect() const;
		float		GetFovX() const;
		float		GetFovY() const;

		// Get near and far plane dimensions in view space coordinates
		float		GetNearWindowWidth() const;
		float		GetNearWindowHeight() const;
		float		GetFarWindowWidth() const;
		float		GetFarWindowHeight() const;

		// Set frustum
		void		SetLens(float fovY, float aspect, float nearZ, float farZ);

		// Define camera space via LookAt parameters.
		void		LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
		void		LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

		// Get View/Proj matrices
		XMMATRIX	GetView() const;
		XMMATRIX	GetProj() const;
		XMMATRIX	GetViewProj() const;

		XMFLOAT4X4	GetView4x4f() const;
		XMFLOAT4X4	GetProj4x4f() const;
		XMFLOAT4X4	GetViewProj4x4f() const;

		// Strafe/Walk the camera a distance d.
		void		StrafeX(float d);
		void		StrafeY(float d);
		void		Walk(float d);

		// Rotate the camera.
		void		Pitch(float angle);
		void		RotateY(float angle);

		void		SetPosition(float x, float y, float z);


	private:

		XMFLOAT4X4		m_viewMatrix;
		XMFLOAT4X4		m_viewProjMatrix;

		float			m_phi{ 0.2f * XM_PI };
		float			m_theta{ 1.5f * XM_PI };
		float			m_radius{ 15.0f };

		XMFLOAT3		m_position{ 0.0f,0.0f,0.0f };
		XMFLOAT3		m_right{ 0.1f,0.0f,0.0f };
		XMFLOAT3		m_up{ 0.0f,1.0f,0.0f };
		XMFLOAT3		m_look{ 0.0f,0.0f,1.0f };

		// Frustum properties
		float			m_nearZ{ 1.0f };
		float			m_farZ{ 1000.f };
		float			m_aspect{ 0.0f };
		float			m_fovY{ 0.0f };
		float			m_nearWindowHeight{ 0.0f };
		float			m_farWindowHeight{ 0.0f };

		bool			m_viewDirty{ true };
	};
}

#endif