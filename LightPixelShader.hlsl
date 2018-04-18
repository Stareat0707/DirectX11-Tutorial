Texture2D shaderTexture;
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	// �� �ؽ�ó ��ǥ ��ġ���� Sampler�� ����� �ؽ�ó�� �ȼ� ���� ���ø��մϴ�.
	float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

	// ����� ���� ���� ������ ������ŵ�ϴ�.
	float3 lightDir = -lightDirection;

	// �� �ȼ��� ���� ���� ����Ѵ�.
	float lightIntensity = saturate(dot(input.normal, lightDir));

	// ���� ������ ���յ� Ȯ��� ���� �����Ͽ� ���� Ȯ��� ���� ���� �����Ѵ�.
	float4 color = saturate(diffuseColor * lightIntensity);

	// �ؽ�ó �ȼ��� ���� Ȯ�� ���� ���ϰ� ���� �ȼ� �� ����� ����ϴ�.
	color = color * textureColor;

	return color;
}
