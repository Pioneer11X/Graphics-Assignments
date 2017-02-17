#pragma once

#include "DirectXMath.h"

using namespace DirectX;

class Camera
{

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 position;
	XMFLOAT3 direction;

	bool dirty;

	float rotationX;
	float rotationY;

public:

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();

	void Update();

	void MoveForward(float factor);

	Camera(XMFLOAT3 _position,float _rotationX, float _rotationY);
	~Camera();
};

