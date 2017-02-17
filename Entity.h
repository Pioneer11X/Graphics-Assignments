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

	// Looks like these are mandatory kind of deal.
	XMFLOAT4X4 positionMatrix;
	XMFLOAT4X4 rotationMatrix;
	XMFLOAT4X4 scaleMatrix; 

	bool dirty; // This is true if the one of the components have been changed and we need to recreate the WorldMatrix

	Mesh* entityMesh;

public:

	void UpdateWorldMatrix();

	Mesh * GetMesh();
	XMFLOAT4X4 GetWorldMatrix();

	void SetTranslation(float _x, float _y, float _z);
	void SetRotation(float _rotationZ);
	void SetScale(float _x, float _y, float _z);

	Entity(Mesh * _entityMesh);
	~Entity();
};

