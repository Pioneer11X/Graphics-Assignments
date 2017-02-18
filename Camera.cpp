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

Camera::Camera(XMFLOAT3 _position, float _rotationX, float _rotationY, int _width, int _height)
{
	position = _position;
	rotationX = _rotationX;
	rotationY = _rotationY;

	SetProjectionMatrix(_width, _height);

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

void Camera::IncrementRotationX(float factor)
{
	if (rotationX + factor < 0.3f & rotationX + factor > -0.3f) {
		rotationX += factor;
		dirty = true;
	}
}

void Camera::IncrementRotationY(float factor)
{
	//if (rotationY + factor < 0.6f & rotationY + factor > -0.6f) {
		rotationY += factor;
		dirty = true;
	//}
}

void Camera::SetProjectionMatrix(int _width, int _height)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)_width / _height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


Camera::~Camera()
{
}
