#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;

	class Transform
	{
	public:
		Transform() = default;
		~Transform() = default;

		void XM_CALLCONV SetRotation(FXMVECTOR rotation);

		void XM_CALLCONV SetScale(FXMVECTOR scale);

		void XM_CALLCONV SetTranslation(FXMVECTOR translation);

		void			 SetRotation(float x, float y, float z);

		void			 SetScale(float x, float y, float z);

		void			 SetTranslation(float x, float y, float z);

		XMVECTOR		 GetRotation() const;

		XMVECTOR		 GetScale() const;

		XMVECTOR		 GetTranslation() const;

		XMFLOAT3		 GetRotation3f() const;

		XMFLOAT3		 GetScale3f() const;

		XMFLOAT3		 GetTranslation3f() const;

		bool			 IsDirty() const;

		void			 SetDirty(bool dirty);

	private:

		XMFLOAT3			m_rotation{0.0f,0.0f,0.0f};
		
		XMFLOAT3			m_scale{1.0f,1.0f,1.0f};

		XMFLOAT3			m_translation{0.0f,0.0f,0.0f};

		// Indicates if this component is dirty or not
		bool				m_isDirty{ false };

	};
}

#endif