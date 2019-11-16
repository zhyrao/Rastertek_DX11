#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
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
		MessageBox(hWnd, "Could not init Direct3D", "Error", MB_OK);
		return false;
	}

	// create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
		return false;

	//set the initial of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.f);

	// create the model class
	m_Model = new ModelClass;
	if (!m_Model)
		return false;

	// init the model class
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/texture/seafloor.dds");
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	// create the color shader object
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
		return false;

	// init the color shaer
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	// init texture shader
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;
	
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::ShutDown()
{
	if (m_TextureShader)
	{
		m_TextureShader->ShutDown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_ColorShader)
	{
		m_ColorShader->ShutDown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Model)
	{
		m_Model->ShutDown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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
	bool result;
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	// clear buffers to begin the scene
	m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// genrate the view matrix based on the camera's position
	m_Camera->Render();

	// get the world, view, projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// put the model vertex and index buffers on the graphics pipeline to prepare then for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

// 	// render the model using the color shader
// 	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
// 	if (!result)
// 		return false;

	// render with texture
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureSRV());
	if (!result)
		return false;

	// present the rendered scene to the screen
	m_D3D->EndScene();

	

	return true;
}
