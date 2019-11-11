#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

	// create direct3d object
	m_D3D = new D3DClass;
	if (!m_D3D)
		return false;

	// initialize the d3d object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::ShutDown()
{
	if (m_D3D)
	{
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	bool result;

	// render the graphics scene
	result = Render();
	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	// clear buffers to begin the scene
	static float g = 0.0f;
	static bool increase = true;
	if (g >= 1.1f)
	{
		increase = false;
	}

	if (g <= -1.1f)
	{
		increase = true;
	}

	g += increase ? 0.005f : -0.005f;
	m_D3D->BeginScene(0.5f, g, 0.1f, 1.0f);

	// present the rendered scene to the screen
	m_D3D->EndScene();

	

	return true;
}
