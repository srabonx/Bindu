#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"
#include "MeshObject.h"
#include "LightData.h"

namespace BINDU
{
	enum class LightType
	{
		DIRECTIONAL,
		POINT,
		SPOT
	};

	class Light : public MeshObject
	{
	public:
		Light() = default;
		~Light() = default;

		void					SetColor(const XMFLOAT3 color)
		{
			m_color = color;
		}

		virtual LightData		GetLightData() = 0;

		virtual LightType		GetLightType() = 0;

		virtual bool			IsWithinRange(const XMFLOAT3& position) = 0;

	protected:
		XMFLOAT3		m_color = { 0.5f, 0.5f, 0.5f };
		LightType		m_type;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() = default;
		explicit DirectionalLight(XMFLOAT3 direction) : m_direction(direction){}

		LightData GetLightData() override
		{
			LightData lb;
			lb.Strength = m_color;
			lb.Direction = m_direction;

			return lb;
		}

		void	  SetDirection(const XMFLOAT3& direction)
		{
			m_direction = direction;
		}

		LightType GetLightType() override
		{
			return m_type;
		}

		bool IsWithinRange(const XMFLOAT3& position) override
		{
			return true;
		}

	private:

		XMFLOAT3			m_direction{ 0.0f,-1.0f,0.0f };
		LightType			m_type = LightType::DIRECTIONAL;
	};


	class PointLight : public Light
	{
	public:
		PointLight() = default;
		PointLight(float falloff_start, float falloff_end)
			: m_falloffStart(falloff_start),
			  m_falloffEnd(falloff_end)
		{
		}

		void	  SetFalloff(float start, float end)
		{
			m_falloffStart = start;
			m_falloffEnd = end;
		}

		void	 SetFalloffStart(float start)
		{
			m_falloffStart = start;
		}

		void	 SetFalloffEnd(float end)
		{
			m_falloffEnd = end;
		}

		LightData GetLightData() override
		{
			LightData lb;
			lb.Strength = m_color;
			lb.Position = m_transformComponent.GetTranslation3f();
			lb.FalloffStart = m_falloffStart;
			lb.FalloffEnd = m_falloffEnd;

			return lb;
		}

		LightType GetLightType() override
		{
			return m_type;
		}

		bool IsWithinRange(const XMFLOAT3& position) override
		{
			auto lightPos = m_transformComponent.GetTranslation3f();

			float dx = lightPos.x - position.x;
			float dy = lightPos.y - position.y;
			float dz = lightPos.z - position.z;

			float distanceSquared = dx * dx + dy * dy + dz * dz;

			float range = m_falloffEnd;

			return distanceSquared <= (range * range);
		}

	private:

		float				m_falloffStart{ 5.0f };

		float				m_falloffEnd{ 0.0f };

		LightType			m_type = LightType::POINT;
	};

	class SpotLight : public Light
	{
	public:

		SpotLight() = default;

		SpotLight(const XMFLOAT3& direction, float spot_power, float falloff_start, float falloff_end)
			: m_direction(direction),
			  m_spotPower(spot_power),
			  m_falloffStart(falloff_start),
			  m_falloffEnd(falloff_end)
		{
		}


		void	  SetDirection(const XMFLOAT3& direction)
		{
			m_direction = direction;
		}

		void	 SetSpotPower(float power)
		{
			m_spotPower = power;
		}

		void	  SetFalloff(float start, float end)
		{
			m_falloffStart = start;
			m_falloffEnd = end;
		}

		void	 SetFalloffStart(float start)
		{
			m_falloffStart = start;
		}

		void	 SetFalloffEnd(float end)
		{
			m_falloffEnd = end;
		}

		LightData GetLightData() override
		{
			LightData lb;
			lb.Strength = m_color;
			lb.Position = m_transformComponent.GetTranslation3f();
			lb.Direction = m_direction;
			lb.FalloffStart = m_falloffStart;
			lb.FalloffEnd = m_falloffEnd;
			lb.SpotPower = m_spotPower;

			return lb;
		}

		LightType GetLightType() override
		{
			return m_type;
		}

		bool IsWithinRange(const XMFLOAT3& position) override
		{
			auto lightPos = m_transformComponent.GetTranslation3f();

			float dx = lightPos.x - position.x;
			float dy = lightPos.y - position.y;
			float dz = lightPos.z - position.z;

			float distanceSquared = dx * dx + dy * dy + dz * dz;

			float range = m_falloffEnd;

			return (distanceSquared <= (range * range))&& IsWithinSpotLightCone(position);
		}

	private:
		bool IsWithinSpotLightCone(const XMFLOAT3& position) const
		{
			// Get the spotlight position and direction
			XMFLOAT3 spotlightPos = m_transformComponent.GetTranslation3f();
			XMFLOAT3 spotlightDir = m_transformComponent.GetForwardVector3f();

			// Get the object's position
			XMFLOAT3 objectPos = position;

			// Compute the vector from the spotlight to the object
			XMFLOAT3 toObjectVec = {
				objectPos.x - spotlightPos.x,
				objectPos.y - spotlightPos.y,
				objectPos.z - spotlightPos.z
			};

			// Normalize the vector to the object
			XMVECTOR toObject = XMVector3Normalize(XMLoadFloat3(&toObjectVec));
			XMVECTOR spotlightDirection = XMLoadFloat3(&spotlightDir);

			// Compute the dot product between the spotlight's forward direction and the vector to the object
			float dotProduct = XMVectorGetX(XMVector3Dot(spotlightDirection, toObject));

			// Calculate the spotlight's cosine of the cutoff angle
			float spotlightConeAngle = 0.785398f; // In radians
			float cosConeAngle = cosf(spotlightConeAngle);

			// If the dot product is greater than the cosine of the cone angle, the object is inside the cone
			return dotProduct >= cosConeAngle;
		}

	private:

		XMFLOAT3			m_direction{ 0.0f,-1.0f,0.0f };

		float				m_spotPower{ 10.f };

		float				m_falloffStart{ 5.0f };

		float				m_falloffEnd{ 10.0f };

		LightType			m_type = LightType::SPOT;
	};

}

#endif