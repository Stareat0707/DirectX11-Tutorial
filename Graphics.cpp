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

	// Direct3D 객체를 생성합니다.
	m_d3d = new D3D;
	if (!m_d3d)
	{
		return false;
	}

	// Direct3D 객체를 초기화한다
	result = m_d3d->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_camera = new Camera;
	if (!m_camera)
	{
		return false;
	}

	// 카메라의 초기 위치 설정
	m_camera->setPosition(0.0f, 0.0f, -10.0f);

	// 모델 객체 생성
	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// 모델 객체를 초기화합니다.
	result = m_model->initialize(m_d3d->getDevice(), const_cast<char*>("../Engine/data/Cube.txt"), const_cast<WCHAR*>(L"../Engine/data/seafloor.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	// 조명 셰이더 객체를 생성합니다.
	m_lightShader = new LightShader;
	if (!m_lightShader)
	{
		return false;
	}

	// 조명 셰이더 객체를 초기화합니다.
	result = m_lightShader->initialize(m_d3d->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	// 조명 객체를 생성합니다.
	m_light = new Light;
	if (!m_light)
	{
		return false;
	}

	// 조명 객체를 초기화합니다.
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void Graphics::shutdown()
{
	// 조명 객체를 반환합니다.
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// 조명 셰이더 객체를 반환합니다.
	if (m_lightShader)
	{
		m_lightShader->shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	// 모델 객체를 반환합니다.
	if (m_model)
	{
		m_model->shutdown();
		delete m_model;
		m_model = 0;
	}

	// 카메라 객체를 반환합니다.
	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	// D3D 객체를 반환합니다.
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

	// 각 프레임의 회전 변수를 업데이트합니다.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// 그래픽 렌더링을 수행합니다.
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

	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지웁니다.
	m_d3d->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_camera->render();

	//카메라 및 d3d 개체에서 월드, 보기 및 투영 매트릭스를 가져옵니다.
	D3DXMATRIX viewMatrix;
	m_camera->getViewMatrix(viewMatrix);
	D3DXMATRIX worldMatrix;
	m_d3d->getWolrdMatrix(worldMatrix);
	D3DXMATRIX projectionMatrix;
	m_d3d->getProjectionMatrix(projectionMatrix);

	// 삼각형이 회전하도록 회전 값으로 월드 행렬을 회전합니다.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// 모델 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 도면에 사용할 수 있도록 준비합니다.
	m_model->render(m_d3d->getDeviceContext());

	// 조명 셰이더를 사용하여 모델을 렌더링합니다.
	result = m_lightShader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTexture(), m_light->getDirection(), m_light->getDiffuseColor());
	if (!result)
	{
		return false;
	}

	// 렌더링된 장면을 화면에 표시합니다.
	m_d3d->endScene();

	return true;
}
