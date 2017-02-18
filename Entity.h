#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Lights.h"
#include "vector"

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
	Material* entityMaterial;


public:

	void UpdateWorldMatrix();

	Mesh * GetMesh();
	XMFLOAT4X4 GetWorldMatrix();

	void SetTranslation(float _x, float _y, float _z);
	void SetRotation(float _rotationZ);
	void SetScale(float _x, float _y, float _z);

	void MoveRight(float factor);
	void MoveUp(float factor);
	void MoveRightUsingMatrix(float factor);
	void MoveUpUsingMatrix(float factor);

	void PrepareShaders(Camera * _camera, std::vector<DirectionalLight> _directionalLights);

	Entity(Mesh * _entityMesh, Material* _entityMaterial, XMFLOAT3 _position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(0.5f, 0.5f, 0.5f));
	~Entity();
};

