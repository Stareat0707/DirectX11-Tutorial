#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int& screenWidth, int& screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D 객체를 생성합니다.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D 객체를 초기화한다
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라의 초기 위치 설정
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// 모델 객체를 초기화합니다.
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	// 컬러 셰이더 객체 생성
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// 컬러 셰이더 객체를 초기화합니다.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// 컬러 셰이더 객체를 반환합니다.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// 모델 객체를 반환합니다.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// 카메라 객체를 반환합니다.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// D3D 객체를 반환합니다.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	// 그래픽 렌더링을 수행합니다.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지웁니다.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	//카메라 및 d3d 개체에서 월드, 보기 및 투영 매트릭스를 가져옵니다.
	D3DXMATRIX viewMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	D3DXMATRIX worldMatrix;
	m_D3D->GetWolrdMatrix(worldMatrix);
	D3DXMATRIX projectionMatrix;
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 모델 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 도면에 사용할 수 있도록 준비합니다.
	m_Model->Render(m_D3D->GetDeviceContext());

	// 컬러 셰이더를 사용하여 모델을 렌더링합니다.
	bool result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 렌더링된 장면을 화면에 표시합니다.
	m_D3D->EndScene();

	return true;
}
