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

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ�Ѵ�.
	int screenWidth = 0;
	int screenHeight = 0;

	// �������� api�� ����Ͽ� �ʱ�ȭ�Ѵ�.
	initializeWindows(screenWidth, screenHeight);

	// input ��ü�� �����մϴ�. �� ��ü�� �����κ��� ������ Ű���� �Է��� ó���ϱ� ���� ����մϴ�.
	m_input = new Input;
	if (!m_input)
	{
		return false;
	}

	// input ��ü�� �ʱ�ȭ�մϴ�.
	m_input->initialize();

	// graphics ��ü�� �����մϴ�. �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� �մϴ�.
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
	// Graphics ��ü�� ��ȯ�մϴ�.
	if (m_graphics)
	{
		m_graphics->shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	// Input ��ü�� ��ȯ�մϴ�.
	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	// â�� �����ŵ�ϴ�.
	shutdownWindows();

	return;
}

void System::run()
{
	bool result;

	// �޼��� ����ü�� �ʱ�ȭ�մϴ�.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޼����� ���� ������ ������ ���ϴ�.
	bool done = false;
	while (!done)
	{
		// ������ �޼����� ó���մϴ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ���������ϴ�.
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

	// ������ Esc Ű�� ���� ���ø����̼��� �����ϱ⸦ ���ϴ��� Ȯ���մϴ�.
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
	// �ܺ� �����͸� �� ��ü�� �����մϴ�.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �����ɴϴ�.
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� �����մϴ�.
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����ϴ�.
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

	// ������ Ŭ������ ����մϴ�.
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� �˾ƿɴϴ�.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ������ �մϴ�.
	int posX, posY;
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �մϴ�.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ������ �մϴ�.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ���� ���� ���� ���ݴϴ�.
		posX = posY = 0;
	}
	else
	{
		// ������ ����� 800x600�� ũ�⸦ ������ �մϴ�.
		screenWidth = 800;
		screenHeight = 600;

		// â�� ������� �߾ӿ� ������ �մϴ�.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� �������ϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ݴϴ�.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ �ʽ��ϴ�.
	ShowCursor(false);

	return;
}

void System::shutdownWindows()
{
	// ���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� �����մϴ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ �����մϴ�.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// �����찡 ���ŵǾ����� Ȯ���մϴ�.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		
		// �����찡 �������� Ȯ���մϴ�.
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
