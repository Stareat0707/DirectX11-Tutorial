Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	// �� �ؽ�ó ��ǥ ��ġ���� Sampler�� ����� �ؽ�ó�� �ȼ� ���� ���ø��մϴ�.
	float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

	return textureColor;
}
