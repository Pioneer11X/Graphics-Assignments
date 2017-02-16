#pragma once
#include "Mesh.h"

using namespace DirectX;

class Entity
{

	// World Matrix, Individual Position, Rotation and Scale Vectors.
	XMFLOAT4X4 worldMatrix;

	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	bool dirty; // This is true if the one of the components have been changed and we need to recreate the WorldMatrix

	Mesh* entityMesh;

public:

	void UpdateWorldMatrix();

	Mesh * GetMesh();
	XMFLOAT4X4 GetWorldMatrix();

	Entity(Mesh * _entityMesh);
	~Entity();
};

