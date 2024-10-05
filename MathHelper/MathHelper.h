#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b-a);
	}

    static int Rand(int a, int b)
    {
        return a + rand() % ((b - a) + 1);
    }

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	 
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return DirectX::XMVectorSet(
			radius*sinf(phi)*cosf(theta),
			radius*cosf(phi),
			radius*sinf(phi)*sinf(theta),
			1.0f);
	}

    static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
        DirectX::XMMATRIX A = M;
        A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

        DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
        return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}

    static DirectX::XMFLOAT4X4 Identity4x4()
    {
        static DirectX::XMFLOAT4X4 I(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return I;
    }

    static DirectX::XMVECTOR RandUnitVec3();
    static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	static const float Infinity;
	static const float Pi;

	static DirectX::XMFLOAT3 XM_CALLCONV QuaternionToEular(DirectX::FXMVECTOR quaternion)
	{
		DirectX::XMFLOAT4 quaternion4f = {};
		DirectX::XMStoreFloat4(&quaternion4f, quaternion);
		return QuaternionToEular(quaternion4f);
	}

	static DirectX::XMMATRIX CreateRotationMatrixFromQuaternion(DirectX::FXMVECTOR quaternion)
	{
		DirectX::XMFLOAT4 quaternion4f = {};
		DirectX::XMStoreFloat4(&quaternion4f, quaternion);

		return CreateRotationMatrixFromQuaternion(quaternion4f);
	}

	static DirectX::XMFLOAT3 QuaternionToEular(const DirectX::XMFLOAT4& quaternion)
	{
		float x = quaternion.x;
		float y = quaternion.y;
		float z = quaternion.z;
		float w = quaternion.w;

		float yaw = atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (y * y + z * z));
		float pitch = asin(2.0f * (w * z - x * y));
		float roll = atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + z * z));

		return { roll,pitch,yaw };
	}

	static DirectX::XMMATRIX CreateRotationMatrixFromQuaternion(const DirectX::XMFLOAT4& quaternion)
	{
		DirectX::XMFLOAT3 eularAngles = QuaternionToEular(quaternion);

		float roll = eularAngles.x;
		float pitch = eularAngles.y;
		float yaw = eularAngles.z;

		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	}


};

#endif


