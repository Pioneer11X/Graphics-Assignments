#include "Camera.h"



XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::Update()
{
	if (dirty) {
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotationX, rotationY, 0.0f);
		XMVECTOR directionVector = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);

		XMStoreFloat3(&direction, directionVector);

		XMMATRIX viewMatrixMatrix = XMMatrixLookToLH(
			XMLoadFloat3(&position),
			XMLoadFloat3(&direction),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);

		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(viewMatrixMatrix));

		dirty = false;
	}
}

void Camera::MoveForward(float factor)
{
	// We can add the current direction the camera is looking at to the current position.
	position.x += direction.x * factor;
	position.y += direction.y * factor;
	position.z += direction.z * factor;
	dirty = true;
}

Camera::Camera(XMFLOAT3 _position, float _rotationX, float _rotationY)
{
	position = _position;
	rotationX = _rotationX;
	rotationY = _rotationY;

	dirty = true;
	Update();

}

void Camera::MoveLeft(float factor)
{
	/*
	Left/right vectors can be found using the cross product of forward (camera�s direction) and the world�s up axis (0,1,0).
	*/
	XMVECTOR LeftSide;
	LeftSide = XMVector3Cross(XMLoadFloat3(&direction), XMVectorSet(0, 1, 0, 0));
	LeftSide = XMVector3Normalize(LeftSide);

	XMFLOAT3 LeftSideFloat;
	XMStoreFloat3(&LeftSideFloat, LeftSide);

	position.x += LeftSideFloat.x * factor;
	position.y += LeftSideFloat.y * factor;
	position.z += LeftSideFloat.z * factor;

	dirty = true;

}


Camera::~Camera()
{
}
