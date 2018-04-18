#include "Model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	// �ﰢ���� ������ �����ϴ� ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::shutdown()
{
	// �� �ؽ�ó�� �����մϴ�.
	releaseTexture();

	// ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	shutdownBuffers();

	return;
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	// ���� ���ۿ� �ε��� ���۸� �׷��Ƚ� ���������ο� �־� ȭ�鿡 �׸� �غ� �մϴ�.
	renderBuffers(deviceContext);

	return;
}

int Model::getIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::getTexture()
{
	return m_texture->getTexture();
}

bool Model::initializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	// ���� �迭�� ���̸� �����մϴ�.
	m_vertexCoount = 3;

	// �ε��� �迭�� ���̸� �����մϴ�.
	m_indexCount = 3;

	// ���� �迭�� �����մϴ�.
	VertexType* vertices = new VertexType[m_vertexCoount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� �����մϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���� �迭�� ���� �ֽ��ϴ�.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);	// ���� �Ʒ�
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	
	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��� ���
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);	// ������ �Ʒ�
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// �ε��� �迭�� ���� �ֽ��ϴ�.
	indices[0] = 0;	// ���� �Ʒ�
	indices[1] = 1;	// ��� ���
	indices[2] = 2;	// ������ �Ʒ�

	// ���� ������ description�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCoount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ���� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���۸� �����մϴ�.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ε��� ������ description�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::shutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler�� ���� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ� �մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// input assembler�� �ε��� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ� �մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ���Դϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Model::loadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// �ؽ��� ��ü�� �����մϴ�.
	m_texture = new Texture;
	if (!m_texture)
	{
		return false;
	}

	// �ؽ��� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_texture->initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::releaseTexture()
{
	// �ؽ��� ��ü�� �����մϴ�.
	if (m_texture)
	{
		m_texture->shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}
