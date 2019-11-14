#include "systemclass.h"



SystemClass::SystemClass()
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	// initilialize the window api
	InitializeWindows(screenWidth, screenHeight);

	// create the input
	m_Input = new InputClass();
	if (!m_Input)
		return false;

	// initialize the input
	m_Input->Initialize();

	// create the graphics object
	m_Graphics = new GraphicsClass();
	if(!m_Graphics)
	{
		return false;
	}

	// initialize the graphics
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!result)
		return false;

	return true;
}

void SystemClass::ShutDown()
{
	// release the graphics
	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// release the input class
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}
	// shut down windows
	ShutDownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// initilize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// loop until there is a quit message from the window to the user
	done = false;
	while (!done)
	{
		// handle the windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// if windows signals to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// otherwise do the frame processing
			result = Frame();
			if (!result)
				done = true;
		}
	}

}

LRESULT SystemClass::MessageHander(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
	{
		m_Input->KeyDown((unsigned int)wParam);
		return 0;
	}
	case WM_KEYUP:
	{
		m_Input->KeyUp((unsigned int)wParam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	}
	return LRESULT();
}

bool SystemClass::Frame()
{
	bool result;

	// check the user pressed escape and exit
	if (m_Input->IsKeyDown(VK_ESCAPE))
		return false;

	// do the frame processing for graphics object
	result = m_Graphics->Frame();
	if (!result)
		return false;

	return true;
}

void SystemClass::InitializeWindows(int & width, int & height)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// get an external pointer to this object
	ApplicationHandle = this;

	// get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// give the application name
	m_ApplicationName = "Rastertek DX11";

	// setup the window class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// register the window class
	RegisterClassEx(&wc);

	// determine the resolution of the clients desktop screen
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	// setup the screen setting depending on weather it is 
	// running in full screen or in windowed mode
	if (FULL_SCREEN)
	{
		// if full screen set the screen to maximum size of the users desktop or 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 23;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// change the display setting to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		// if windowed then set it to 800x600 reslution
		width = 800;
		height = 600;

		// place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// create the window with the screen settings and get the handle to it
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, m_hInstance, NULL);

	// bring the window up on the screen and set it as main focus
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// hide the mouse cursor
	ShowCursor(false);

	return;
}

void SystemClass::ShutDownWindows()
{
	// show window cursor
	ShowCursor(true);

	// fix the display setting if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// remove the window
	DestroyWindow(m_hWnd);

	// remove the applcation instance
	UnregisterClass(m_ApplicationName, m_hInstance);

	// release the pointer to this class
	ApplicationHandle = NULL;

	return;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return ApplicationHandle->MessageHander(hWnd, msg, wParam, lParam);
	}
	}
}
