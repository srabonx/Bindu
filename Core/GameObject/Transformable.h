#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;

	class Transformable
	{
	public:
		virtual		~Transformable() {}

		void		SetRotation(float x, float y, float z)
		{
			m_rotation = { x,y,z };
		}

		void		SetTranslation(float x, float y, float z)
		{
			m_translation = { x,y,z };
		}

		void		SetScale(float x, float y, float z)
		{
			m_scale = { x,y,z };
		}

	protected:
		XMFLOAT3			m_rotation{ 0.0f,0.0f,0.0f };

		XMFLOAT3			m_scale{ 1.0f,1.0f,1.0f };

		XMFLOAT3			m_translation{ 0.0f,0.0f,0.0f };
	};
}

#endif