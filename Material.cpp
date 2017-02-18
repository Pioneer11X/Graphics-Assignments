#include "Material.h"



SimplePixelShader * Material::GetPixelShader()
{
	return pixShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertShader;
}

ID3D11ShaderResourceView * Material::GetShaderResourceView()
{
	return SRV;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return samplerState;
}

Material::Material(SimpleVertexShader * _vertShader, SimplePixelShader * _pixelShader, ID3D11ShaderResourceView* _SRV, ID3D11SamplerState* _samplerState)
{
	vertShader = _vertShader;
	pixShader = _pixelShader;

	SRV = _SRV;
	samplerState = _samplerState;

}

Material::~Material()
{
	
}
