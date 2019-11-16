// constant buffer
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// input type
struct VertexInputType
{
	float4 position:		POSITION;
	float2 textureCoord:	TEXCOORD0;
};

// pixel input type
struct PixelInputType
{	
	float4 Position:		SV_POSITION;
	float2 texCoord:		TEXCOORD0;
};


// vs shader
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// position transform
	input.position.w = 1.0f;

	output.Position = mul(input.position, worldMatrix);
	output.Position = mul(output.Position, viewMatrix);
	output.Position = mul(output.Position, projectionMatrix);

	output.texCoord = input.textureCoord;

	return output;
}