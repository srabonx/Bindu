#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <DirectXMath.h>

#include "../../MathHelper/MathHelper.h"

namespace BINDU
{
	using namespace DirectX;

	class Transform
	{
	public:
		Transform() = default;
		~Transform() = default;

		void			 Update();

		void XM_CALLCONV SetRotation(FXMVECTOR rotation);

		void XM_CALLCONV SetScale(FXMVECTOR scale);

		void XM_CALLCONV SetTranslation(FXMVECTOR translation);

		void XM_CALLCONV SetTransform(FXMMATRIX transform);

		void			 SetRotation(float x, float y, float z);

		void			 SetScale(float x, float y, float z);

		void			 SetTranslation(float x, float y, float z);

		void			 SetTransform(const XMFLOAT4X4& transform);

		XMVECTOR		 GetRotation() const;

		XMVECTOR		 GetScale() const;

		XMVECTOR		 GetTranslation() const;

		XMMATRIX		 GetTransform() const;

		XMFLOAT3		 GetRotation3f() const;

		XMFLOAT3		 GetScale3f() const;

		XMFLOAT3		 GetTranslation3f() const;

		XMFLOAT4X4		 GetTransform4x4f() const;

		XMVECTOR		 GetForwardVector() const;

		XMFLOAT3		 GetForwardVector3f() const;

		bool			 IsDirty() const;

		void			 SetDirty(bool dirty);

	private:

		XMFLOAT4X4			m_transform = MathHelper::Identity4x4();

		XMFLOAT3			m_rotation{ 0.0f,0.0f,0.0f };
		
		XMFLOAT3			m_scale{1.0f,1.0f,1.0f};

		XMFLOAT3			m_translation{0.0f,0.0f,0.0f};

		bool				m_setTransform{ false };

		// Indicates if this component is dirty or not
		bool				m_isDirty{ false };
		std::uint8_t		m_dirtyFrameCount{ 3 };

	};
}

#endif