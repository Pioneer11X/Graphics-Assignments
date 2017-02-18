#include "Entity.h"
#include "Lights.h"


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

void Entity::PrepareShaders(Camera * _camera, std::vector<DirectionalLight> _directionalLights )
{
	
	SimpleVertexShader * localVertexShader = entityMaterial->GetVertexShader();
	SimplePixelShader * localPixelShader = entityMaterial->GetPixelShader();

	/*localPixelShader->SetData("dlight", &_directionalLight, sizeof(DirectionalLight));*/

	// Fix this use an iterator.
	localPixelShader->SetData("dlight", &_directionalLights[0], sizeof(DirectionalLight));
	localPixelShader->SetData("dlight2", &_directionalLights[1], sizeof(DirectionalLight));

	localVertexShader->SetMatrix4x4("world", this->GetWorldMatrix());
	localVertexShader->SetMatrix4x4("view", _camera->GetViewMatrix());
	localVertexShader->SetMatrix4x4("projection", _camera->GetProjectionMatrix());
	localVertexShader->CopyAllBufferData();
	localVertexShader->SetShader();

	localPixelShader->SetFloat3("directionalLightDirection", XMFLOAT3(1, 0, 0));
	localPixelShader->SetFloat4("directionalLightColor", XMFLOAT4(1, 0.1f, 0.1f, 1));
	localPixelShader->SetFloat3("pointLightPosition", XMFLOAT3(2, 2, 0));
	localPixelShader->SetFloat4("pointLightColor", XMFLOAT4(0.1, 0.1f, 1, 1));
	localPixelShader->SetFloat3("cameraPosition", XMFLOAT3(0, 0, -5));

	localPixelShader->SetShaderResourceView("srv", entityMaterial->GetShaderResourceView());
	localPixelShader->SetSamplerState("samp", entityMaterial->GetSamplerState());

	localPixelShader->CopyAllBufferData();
	localPixelShader->SetShader();
}

Entity::Entity(Mesh * _entityMesh, Material * _entityMaterial, XMFLOAT3 _position, XMFLOAT3 _rotation, XMFLOAT3 _scale)
{
	entityMaterial = _entityMaterial;
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
