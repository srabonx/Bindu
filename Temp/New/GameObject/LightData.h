#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;

	struct LightData
	{
		XMFLOAT3 Strength = { 0.0f, 0.0f, 0.0f };
		float FalloffStart = 1.0f;                          // point/spot light only
		XMFLOAT3 Direction = { 0.0f, 0.0f, 0.0f };// directional/spot light only
		float FalloffEnd = 10.0f;                           // point/spot light only
		XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
		float SpotPower = 64.0f;                            // spot light only
	};


	struct LightConstants
	{

		int				NumOfDirLight{ 0 };

		int				NumOfPointLight{ 0 };

		int				NumOfSpotLight{ 0 };

		int				CbPad01;

		XMFLOAT4		AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

		LightData		LightData[16];
	};

}

#endif