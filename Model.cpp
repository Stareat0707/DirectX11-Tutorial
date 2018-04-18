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

	// 삼각형의 형상을 유지하는 정점 및 인덱스 버퍼를 초기화합니다.
	result = initializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::shutdown()
{
	// 모델 텍스처를 해제합니다.
	releaseTexture();

	// 정점 버퍼와 인덱스 버퍼를 해제합니다.
	shutdownBuffers();

	return;
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어 화면에 그릴 준비를 합니다.
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

	// 정점 배열의 길이를 설정합니다.
	m_vertexCoount = 3;

	// 인덱스 배열의 길이를 설정합니다.
	m_indexCount = 3;

	// 정점 배열을 생성합니다.
	VertexType* vertices = new VertexType[m_vertexCoount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 생성합니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점 배열에 값을 넣습니다.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);	// 왼쪽 아래
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	
	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 상단 가운데
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);	// 오른쪽 아래
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 인덱스 배열에 값을 넣습니다.
	indices[0] = 0;	// 왼쪽 아래
	indices[1] = 1;	// 상단 가운데
	indices[2] = 2;	// 오른쪽 아래

	// 정점 버퍼의 description을 작성합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCoount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 정점 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼를 생성합니다.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 인덱스 버퍼의 description을 작성합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::shutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler에 정점 버퍼를 활성화하여 그려질 수 있게 합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// input assembler에 인덱스 버퍼를 활성화하여 그려질 수 있게 합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형입니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Model::loadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// 텍스쳐 객체를 생성합니다.
	m_texture = new Texture;
	if (!m_texture)
	{
		return false;
	}

	// 텍스쳐 객체를 초기화합니다.
	result = m_texture->initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::releaseTexture()
{
	// 텍스쳐 객체를 해제합니다.
	if (m_texture)
	{
		m_texture->shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}
