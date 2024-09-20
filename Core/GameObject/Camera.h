#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"


namespace BINDU
{

	struct CameraViewConstant
	{
		XMFLOAT4X4		ViewMatrix;
		XMFLOAT4X4		ProjMatrix;
		XMFLOAT4X4		ViewProjMatrix;
		XMFLOAT4X4		InvViewProjMatrix;
	};

	class Camera : public GameObject
	{
	public:
		Camera();
		~Camera() = default;

		void Initialize(const D3DDeviceManager& deviceManager) override;

		void Update() override;

		void Render(const D3DCommandContext& commandContext, const D3DPipelineStateManager& pipelineManager, const std::string& pipelineName) override;

		void Resize(std::uint16_t width, std::uint16_t height);


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

		XMFLOAT4X4	GetView4x4f() const;
		XMFLOAT4X4	GetProj4x4f() const;

		// Strafe/Walk the camera a distance d.
		void		Strafe(float d);
		void		Walk(float d);

		// Rotate the camera.
		void		Pitch(float angle);
		void		RotateY(float angle);


	private:

		UploadBuffer	m_cameraViewConstants;

		XMFLOAT3		m_eyePos;

		XMFLOAT4X4		m_viewMatrix;
		XMFLOAT4X4		m_projMatrix;

		float			m_phi{0.2f * XM_PI};
		float			m_theta{1.5f * XM_PI};
		float			m_radius{15.0f};

		XMFLOAT3		m_right{0.1f,0.0f,0.0f};
		XMFLOAT3		m_up{0.0f,1.0f,0.0f};
		XMFLOAT3		m_look{0.0f,0.0f,1.0f};

		// Frustum properties
		float			m_nearZ{1.0f};
		float			m_farZ{1000.f};
		float			m_aspect{0.0f};
		float			m_fovY{0.0f};
		float			m_nearWindowHeight{0.0f};
		float			m_farWindowHeight{0.0f};

		bool			m_viewDirty{true};


	};
}

#endif