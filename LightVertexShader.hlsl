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
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input)
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

	// ���� ��Ŀ� ���� ���� ���͸� ����մϴ�.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// �Ϲ� ���͸� ǥ��ȭ�մϴ�.
	output.normal = normalize(output.normal);

	return output;
}
