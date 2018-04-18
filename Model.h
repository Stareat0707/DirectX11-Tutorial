#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

#include "Texture.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool initialize(ID3D11Device*, WCHAR*);
	void shutdown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device*);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext*);
	bool loadTexture(ID3D11Device*, WCHAR*);
	void releaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCoount, m_indexCount;
	Texture* m_texture;
};
