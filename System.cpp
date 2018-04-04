#include "System.h"

System::System()
{
	m_input = 0;
	m_graphics = 0;
}

System::System(const System& other)
{
}

System::~System()
{
}

bool System::initialize()
{
	bool result;

	// 함수의 높이와 너비를 전달하기 전에 변수를 0으로 초기화한다.
	int screenWidth = 0;
	int screenHeight = 0;

	// 윈도우즈 api를 사용하여 초기화한다.
	initializeWindows(screenWidth, screenHeight);

	// input 객체를 생성합니다. 이 객체는 유저로부터 들어오는 키보드 입력을 처리하기 위해 사용합니다.
	m_input = new Input;
	if (!m_input)
	{
		return false;
	}

	// input 객체를 초기화합니다.
	m_input->initialize();

	// graphics 객체를 생성합니다. 이 객체는 이 어플리케이션의 모든 그래픽 요소를 그리는 일을 합니다.
	m_graphics = new Graphics;
	if (!m_graphics)
	{
		return false;
	}

	result = m_graphics->initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void System::shutdown()
{
	// Graphics 객체를 반환합니다.
	if (m_graphics)
	{
		m_graphics->shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	// Input 객체를 반환합니다.
	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	// 창을 종료시킵니다.
	shutdownWindows();

	return;
}

void System::run()
{
	bool result;

	// 메세지 구조체를 초기화합니다.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료 메세지를 받을 때까지 루프를 돕니다.
	bool done = false;
	while (!done)
	{
		// 윈도우 메세지를 처리합니다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나갑니다.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

bool System::frame()
{
	bool result;

	// 유저가 Esc 키를 눌러 어플리케이션을 종료하기를 원하는지 확인합니다.
	if (m_input->isKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_graphics->frame();
	if(!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK System::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		m_input->keyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		m_input->keyUp((unsigned int)wparam);
		return 0;
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void System::initializeWindows(int& screenWidth, int& screenHeight)
{
	// 외부 포인터를 이 객체로 설정합니다.
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져옵니다.
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정합니다.
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춥니다.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 클래스를 등록합니다.
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 알아옵니다.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면 설정을 합니다.
	int posX, posY;
	if (FULL_SCREEN)
	{
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 합니다.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정을 합니다.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면의 왼쪽 위로 마줍니다.
		posX = posY = 0;
	}
	else
	{
		// 윈도우 모드라면 800x600의 크기를 가지게 합니다.
		screenWidth = 800;
		screenHeight = 600;

		// 창을 모니터의 중앙에 오도록 합니다.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들고 그 핸들을 가져오니다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 줍니다.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않습니다.
	ShowCursor(false);

	return;
}

void System::shutdownWindows()
{
	// 마우스 커서를 표시합니다.
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿉니다.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거합니다.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거합니다.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우가 제거되었는지 확인합니다.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		
		// 윈도우가 닫히는지 확인합니다.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApplicationHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
