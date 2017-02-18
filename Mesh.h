#pragma once
#include "Vertex.h"
#include "DXCore.h"
#include "vector"
#include "fstream"

using namespace DirectX;

class Mesh
{

	Vertex* vertices;
	int vertexCount;
	int * indices;
	int indexCount;

	ID3D11Buffer* meshVertexBuffer;
	ID3D11Buffer* meshIndexBuffer;
	ID3D11Device* meshDevice;

public:
	Mesh(Vertex* _vertices, int _vertexCount, int* _indices, int _indexCount, ID3D11Device* _inputDevice);
	Mesh(char* fileName, ID3D11Device* _device);
	~Mesh();

	void SetBuffers();

	ID3D11Buffer* GetIndexBuffer();
	ID3D11Buffer* GetVertexBuffer();

	int GetIndexCount();
	int GetVertexCount();

};

