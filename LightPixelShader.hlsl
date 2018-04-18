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
	// 이 텍스처 좌표 위치에서 Sampler를 사용해 텍스처의 픽셀 색을 샘플링합니다.
	float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

	// 계산을 위해 조명 방향을 반전시킵니다.
	float3 lightDir = -lightDirection;

	// 이 픽셀의 빛의 양을 계산한다.
	float lightIntensity = saturate(dot(input.normal, lightDir));

	// 빛의 강도와 결합된 확산된 색에 기초하여 최종 확산된 색의 양을 결정한다.
	float4 color = saturate(diffuseColor * lightIntensity);

	// 텍스처 픽셀과 최종 확산 색을 곱하고 최종 픽셀 색 결과를 얻습니다.
	color = color * textureColor;

	return color;
}
