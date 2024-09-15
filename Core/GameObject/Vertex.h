#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

namespace BINDU
{
	using namespace DirectX;

	struct Vertex
	{
		XMFLOAT3	Position;
		XMFLOAT4	Color;
	};

}

#endif