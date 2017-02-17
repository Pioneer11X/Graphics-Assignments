#include "Entity.h"



void Entity::UpdateWorldMatrix()
{
	if (dirty) {
		// We calculate all the stuff here.

		XMStoreFloat4x4(&worldMatrix, 
			XMMatrixTranspose(
				XMLoadFloat4x4(&scaleMatrix) * XMLoadFloat4x4(&rotationMatrix) * XMLoadFloat4x4(&positionMatrix)
				 )
			);

		dirty = false;
	}
}

Mesh * Entity::GetMesh()
{
	return entityMesh;
}

XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return worldMatrix;
}

void Entity::SetTranslation( float _x, float _y, float _z)
{
	position.x = _x;
	position.y = _y;
	position.z = _z;

	XMStoreFloat4x4(&positionMatrix, XMMatrixTranslation( position.x, position.y, position.z ));

	dirty = true;
}

void Entity::SetRotation(float _rotationZ)
{
	rotation.z = _rotationZ;

	XMStoreFloat4x4(&rotationMatrix , XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation.z ));

	dirty = true;
}

void Entity::SetScale(float _x, float _y, float _z)
{
	scale = XMFLOAT3(_x, _y, _z);

	XMStoreFloat4x4(&scaleMatrix, XMMatrixScaling(scale.x, scale.y, scale.z));

	dirty = true;
}

void Entity::MoveRight(float factor)
{
	float newPositionX = position.x + factor * 0.0001f;
	SetTranslation(newPositionX, position.y, position.z);
}

void Entity::MoveUp(float factor)
{
	float newPositionY = position.y + factor * 0.0001f;
	SetTranslation(position.x, newPositionY, position.z);
}

void Entity::MoveRightUsingMatrix(float factor)
{
	positionMatrix.m[3][0] += factor * 0.0001f;
	dirty = true;
}

void Entity::MoveUpUsingMatrix(float factor)
{
	positionMatrix.m[3][1] += factor * 0.0001f;
	dirty = true;
}

Entity::Entity(Mesh * _entityMesh, XMFLOAT3 _position, XMFLOAT3 _rotation, XMFLOAT3 _scale)
{
	entityMesh = _entityMesh;
	position = _position;
	rotation = _rotation;
	scale = _scale;

	SetTranslation(position.x, position.y, position.z);
	SetRotation(rotation.z);
	SetScale(scale.x, scale.y, scale.z);
	dirty = true;
	UpdateWorldMatrix();
}


Entity::~Entity()
{
}
