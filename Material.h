#pragma once

#include "DXCore.h"
#include "SimpleShader.h"

using namespace DirectX;

class Material
{

	SimpleVertexShader * vertShader;
	SimplePixelShader * pixShader;

	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* samplerState;

public:

	SimplePixelShader * GetPixelShader();
	SimpleVertexShader * GetVertexShader();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11SamplerState* GetSamplerState();

	Material(SimpleVertexShader * _vertShader, SimplePixelShader* _pixelShader, ID3D11ShaderResourceView* _SRV, ID3D11SamplerState* _samplerState);
	~Material();
};

