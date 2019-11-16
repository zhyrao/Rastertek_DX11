// globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// typedefs
struct VertexInputType
{
	float4 position: POSITION;
	float4 color:	 COLOR;
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 color:	 COLOR;
};

// vertex shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// change the position vector to be 4 units for proper matrix calcluations
	input.position.w = 1.0f;

	// calculate the position of the vertex against the world, view and projection matrix
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// store the input color as the same
	output.color = input.color;

	return output;
}