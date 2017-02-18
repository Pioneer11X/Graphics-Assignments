#pragma once
#include "DirectXMath.h"

using namespace DirectX;

struct DirectionalLight
{
	// Here the vector 3 (XMFLOAT3) has to come last in the struct because the constant buffers should be 16 bit aligned.
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

class Lights
{
public:
	Lights();
	~Lights();
};

