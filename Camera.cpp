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
	position.x += direction.x * factor * 0.001;
	position.y += direction.y * factor * 0.001;
	position.z += direction.z * factor * 0.001;
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


Camera::~Camera()
{
}
