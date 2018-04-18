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
	// 적절한 행렬 계산을 위해 위치 벡터를 4단위로 변경합니다.
	input.position.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대해 정점의 위치를 계산합니다.
	PixelInputType output;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// 픽셀 셰이더의 텍스처 좌표를 저장합니다.
	output.tex = input.tex;

	// 월드 행렬에 대해 법선 벡터만 계산합니다.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// 일반 벡터를 표준화합니다.
	output.normal = normalize(output.normal);

	return output;
}
