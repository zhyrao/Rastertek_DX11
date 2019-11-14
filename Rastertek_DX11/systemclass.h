#pragma once


// PRE-PROCESSING DIFINES
#define WIN32_LEAN_AND_MEAN

// INCLUDES
#include <Windows.h>

// Class includes
#include "inputclass.h"
#include "graphicsclass.h"


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);	
	~SystemClass();

	bool Initialize();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHander(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


private:
	bool Frame();
	void InitializeWindows(int& width, int& height);
	void ShutDownWindows();

private:
	LPCSTR			m_ApplicationName;
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;


	InputClass*		m_Input;
	GraphicsClass*	m_Graphics;
};


static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// GLOBALS
static SystemClass* ApplicationHandle = 0;
