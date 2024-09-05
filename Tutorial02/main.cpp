#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// Creating system object
	System = new SystemClass;

	// Initializing and running System Object
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release System Object
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}

