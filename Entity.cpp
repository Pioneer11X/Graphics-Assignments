#include "Entity.h"



void Entity::UpdateWorldMatrix()
{
	if (dirty) {
		// We calculate all the stuff here.

		XMStoreFloat4x4(&worldMatrix, 
			XMMatrixTranspose(
				XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationAxis( XMVectorSet(0, 0, 1, 0) , rotation.z ) * XMMatrixTranslation( position.x, position.y, position.z )
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

Entity::Entity(Mesh * _entityMesh)
{
	entityMesh = _entityMesh;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	dirty = true;
	UpdateWorldMatrix();
}


Entity::~Entity()
{
}
