cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
	// ������ ��� ����� ���� ��ġ ���͸� 4������ �����մϴ�.
	input.position.w = 1.0f;

	// ����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ����մϴ�.
	PixelInputType output;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// �ȼ� ���̴��� �ؽ�ó ��ǥ�� �����մϴ�.
	output.tex = input.tex;

	return output;
}