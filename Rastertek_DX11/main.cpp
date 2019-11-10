#include <Windows.h>
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	PSTR pScmdLine,
	int cmdShow)
{
	SystemClass *System;
	bool result;

	// create system object
	System = new SystemClass();
	if (!System)
		return 0;

	// initialize  and run the system object
	result = System->Initialize();
	if (result)
		System->Run();

	// shut down and release the system object
	System->ShutDown();
	delete System;
	System = 0;

	return 0;
}