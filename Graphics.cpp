#include "Graphics.h"

Graphics::Graphics()
{
	m_d3d = 0;
	m_camera = 0;
	m_model = 0;
	m_textureShader = 0;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(int& screenWidth, int& screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D ��ü�� �����մϴ�.
	m_d3d = new D3D;
	if (!m_d3d)
	{
		return false;
	}

	// Direct3D ��ü�� �ʱ�ȭ�Ѵ�
	result = m_d3d->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// ī�޶� ��ü ����
	m_camera = new Camera;
	if (!m_camera)
	{
		return false;
	}

	// ī�޶��� �ʱ� ��ġ ����
	m_camera->setPosition(0.0f, 0.0f, -10.0f);

	// �� ��ü ����
	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// �� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_model->initialize(m_d3d->getDevice(), const_cast<WCHAR*>(L"../Engine/data/seafloor.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	// �ؽ��� ���̴� ��ü�� �����մϴ�.
	m_textureShader = new TextureShader;
	if (!m_textureShader)
	{
		return false;
	}

	// �ؽ��� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_textureShader->initialize(m_d3d->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::shutdown()
{
	// �ؽ��� ���̴� ��ü�� ��ȯ�մϴ�.
	if (m_textureShader)
	{
		m_textureShader->shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	// �� ��ü�� ��ȯ�մϴ�.
	if (m_model)
	{
		m_model->shutdown();
		delete m_model;
		m_model = 0;
	}

	// ī�޶� ��ü�� ��ȯ�մϴ�.
	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	// D3D ��ü�� ��ȯ�մϴ�.
	if (m_d3d)
	{
		m_d3d->shutdown();
		delete m_d3d;
		m_d3d = 0;
	}

	return;
}

bool Graphics::frame()
{
	bool result;

	// �׷��� �������� �����մϴ�.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	// �� �׸��⸦ �����ϱ� ���� ������ ������ ����ϴ�.
	m_d3d->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_camera->render();

	//ī�޶� �� d3d ��ü���� ����, ���� �� ���� ��Ʈ������ �����ɴϴ�.
	D3DXMATRIX viewMatrix;
	m_camera->getViewMatrix(viewMatrix);
	D3DXMATRIX worldMatrix;
	m_d3d->getWolrdMatrix(worldMatrix);
	D3DXMATRIX projectionMatrix;
	m_d3d->getProjectionMatrix(projectionMatrix);

	// �� ������ �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ���鿡 ����� �� �ֵ��� �غ��մϴ�.
	m_model->render(m_d3d->getDeviceContext());

	// �ؽ��� ���̴��� ����Ͽ� ���� �������մϴ�.
	bool result = m_textureShader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTexture());
	if (!result)
	{
		return false;
	}

	// �������� ����� ȭ�鿡 ǥ���մϴ�.
	m_d3d->endScene();

	return true;
}
