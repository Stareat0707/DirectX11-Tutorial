#pragma once

#include <D3D11.h>
#include <D3DX11tex.h>	

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool initialize(ID3D11Device*, WCHAR*);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

