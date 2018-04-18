#include "Model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_model = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// �� �����͸� �ε��մϴ�.
	result = loadModel(modelFilename);
	if (!result)
	{
		return false;
	}

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

	// �� �����͸� �����մϴ�.
	releaseModel();

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

	// ���� �迭�� �����մϴ�.
	VertexType* vertices = new VertexType[m_vertexCount];
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

	// ���� �迭�� �ε��� �迭�� �����ͷ� �ε��մϴ�.
	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// ���� ������ description�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
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

bool Model::loadModel(char* filename)
{
	char input;

	// �� ������ ���ϴ�.
	ifstream fin;
	fin.open(filename);

	// ������ ������ ������ �����մϴ�.
	if (fin.fail())
	{
		return false;
	}

	// ���� ī��Ʈ�� ���� �н��ϴ�.
	do
	{
		fin.get(input);
	} while (input != ':');

	// ���� ī��Ʈ�� �н��ϴ�.
	fin >> m_vertexCount;

	// ���� ���� �����ϰ� �ε��� ���� �����մϴ�.
	m_indexCount = m_vertexCount;

	// ���� ���� ī��Ʈ�� ����Ͽ� ���� �����մϴ�.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// �������� ���� �κб��� �о� �ɴϴ�.
	do
	{
		fin.get(input);
	} while (input != ':');
	fin.get();
	fin.get();

	// ���� �����͸� �н��ϴ�.
	for (int i = 0; i < m_vertexCount; ++i)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}
	
	// �� ������ �ݽ��ϴ�.
	fin.close();

	return true;
}

void Model::releaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}
