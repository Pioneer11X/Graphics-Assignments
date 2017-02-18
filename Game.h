#pragma once

#include "Entity.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "vector"
#include "Camera.h"
#include "Lights.h"
#include "WICTextureLoader.h"


class Game
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// అన్ని ఫంక్ట్శన్లు
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void DrawEntity(Entity* _entity);

	// మౌస్ తో వచ్చే ఇన్పుట్లు
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateMatrices();
	void CreateBasicGeometry();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// దీనితో మౌస్ ఇంతకు ముందు ఎక్కడ వుందో చూడొచ్చు.
	POINT prevMousePos;

	Mesh * triangleMesh;
	Mesh * squareMesh;
	Mesh * cubeMesh;
	Mesh * helixMesh;
	Mesh * coneMesh;
	Mesh * cylinderMesh;
	Mesh * torusMesh;
	Mesh * sphereMesh;

	Entity* triangleEntity;
	Entity* triangleEntity1;
	Entity* triangleEntity2;
	Entity* triangleEntity3;
	Entity* triangleEntity4;
	Entity* squareEntity;

	std::vector<Entity *> entityVector;

	Camera* newCamera;
	Material* newMaterial;
	Material* anotherMaterial;

	std::vector<DirectionalLight> directionalLights;

	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* sampler;

	ID3D11ShaderResourceView* anotherSRV;
};

