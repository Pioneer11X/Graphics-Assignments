struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

Texture2D srv		: register(t0);
SamplerState samp	: register(s0);

cbuffer ExternalLightData: register(b0) {
	DirectionalLight dlight;
}

cbuffer AnotherExternalLight: register(b1) {
	DirectionalLight dlight2;
}

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAl;
	float2 uv			: TEXCOORD;
	//float3 worldPos		: WORLDPOS;
};



// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
		
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	// return float4( 0.7f, 0.7f, 0.7f, 1.0f );

	input.normal = normalize(input.normal);

	// Get the Texture Surface Color
	float4 surfaceColor = srv.Sample(samp, input.uv);

	// Get the lights direction.
	float3 lightDir = dlight.Direction;
	// Negate that direction.
	lightDir = -1.0f * lightDir;
	// Normalize it.
	lightDir = normalize(lightDir);

	// N DOT L
	float dotProduct = saturate(dot(input.normal, lightDir));

	// Get the direction for the second light.
	float lightDir2 = normalize(-1.0f * dlight2.Direction);
	// N DOT L
	float dotProduct2 = saturate(dot(input.normal, lightDir2));

	//Return
	return (dlight.DiffuseColor * dotProduct * surfaceColor + dlight.AmbientColor + dotProduct2 * dlight2.DiffuseColor * surfaceColor + dlight2.AmbientColor);
	//return (dlight.DiffuseColor * dotProduct  + dlight.AmbientColor + dotProduct2 * dlight2.DiffuseColor  + dlight2.AmbientColor);

}