#include "Mesh.h"

Mesh::Mesh(Vertex * _vertices, int _vertexCount, int * _indices, int _indexCount, ID3D11Device * _inputDevice)
{
	vertices = _vertices;
	vertexCount = _vertexCount;
	indices = _indices;
	indexCount = _indexCount;
	meshDevice = _inputDevice;

	// Set the buffers.
	SetBuffers();
}

Mesh::~Mesh()
{

	if (meshVertexBuffer) meshVertexBuffer->Release();
	if (meshIndexBuffer) meshIndexBuffer->Release();
}

void Mesh::SetBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA intialVertexData;
	intialVertexData.pSysMem = vertices;

	meshDevice->CreateBuffer(&vbd, &intialVertexData, &meshVertexBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indexCount;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	meshDevice->CreateBuffer(&ibd, &initialIndexData, &meshIndexBuffer);

}

ID3D11Buffer * Mesh::GetIndexBuffer()
{
	return meshIndexBuffer;
}

ID3D11Buffer * Mesh::GetVertexBuffer()
{
	return meshVertexBuffer;
}

int Mesh::GetIndexCount()
{
	return indexCount;
}

int Mesh::GetVertexCount()
{
	return vertexCount;
}
