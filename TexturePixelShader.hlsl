Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	// 이 텍스처 좌표 위치에서 Sampler를 사용해 텍스처의 픽셀 색을 샘플링합니다.
	float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

	return textureColor;
}
