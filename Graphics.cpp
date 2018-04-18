#include "Graphics.h"

Graphics::Graphics()
{
	m_d3d = 0;
	m_camera = 0;
	m_model = 0;
	m_lightShader = 0;
	m_light = 0;
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
	result = m_model->initialize(m_d3d->getDevice(), const_cast<char*>("../Engine/data/Cube.txt"), const_cast<WCHAR*>(L"../Engine/data/seafloor.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	// ���� ���̴� ��ü�� �����մϴ�.
	m_lightShader = new LightShader;
	if (!m_lightShader)
	{
		return false;
	}

	// ���� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_lightShader->initialize(m_d3d->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	// ���� ��ü�� �����մϴ�.
	m_light = new Light;
	if (!m_light)
	{
		return false;
	}

	// ���� ��ü�� �ʱ�ȭ�մϴ�.
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void Graphics::shutdown()
{
	// ���� ��ü�� ��ȯ�մϴ�.
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// ���� ���̴� ��ü�� ��ȯ�մϴ�.
	if (m_lightShader)
	{
		m_lightShader->shutdown();
		delete m_lightShader;
		m_lightShader = 0;
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
	static float rotation = 0.0f;

	// �� �������� ȸ�� ������ ������Ʈ�մϴ�.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// �׷��� �������� �����մϴ�.
	result = render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::render(float rotation)
{
	bool result;

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

	// �ﰢ���� ȸ���ϵ��� ȸ�� ������ ���� ����� ȸ���մϴ�.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// �� ������ �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ���鿡 ����� �� �ֵ��� �غ��մϴ�.
	m_model->render(m_d3d->getDeviceContext());

	// ���� ���̴��� ����Ͽ� ���� �������մϴ�.
	result = m_lightShader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTexture(), m_light->getDirection(), m_light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	// �������� ����� ȭ�鿡 ǥ���մϴ�.
	m_d3d->endScene();

	return true;
}
