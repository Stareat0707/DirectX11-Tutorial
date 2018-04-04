#include "Texture.h"

Texture::Texture()
{
	m_texture = 0;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

bool Texture::initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// 텍스쳐를 로딩합니다.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::shutdown()
{
	// 텍스쳐 리소스를 반환합니다.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Texture::getTexture()
{
	return m_texture;
}
