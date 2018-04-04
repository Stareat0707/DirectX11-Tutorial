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

	// �ؽ��ĸ� �ε��մϴ�.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::shutdown()
{
	// �ؽ��� ���ҽ��� ��ȯ�մϴ�.
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
