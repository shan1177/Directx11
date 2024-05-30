#include "systemclass.h"

// Constructor, initializing object pointers to null
SystemClass::SystemClass()
{
	m_Input = 0;
	m_Application = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initializing width and height of screen to 0 before sending variables into function
	screenWidth = 0;
	screenHeight = 0;

	// Initializing Windows API
	InitializeWindows(screenWidth, screenHeight);

	// Create and Initialize Input Object, used to handle keyboard input from user
	m_Input = new InputClass;

	m_Input->Initialize();

	// Create and Initialize Application Class Object, used to handle rendering all graphics for application
	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// Releasing the application class object
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// Release the input object
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the Window
	ShutdownWindows();
	
	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message stucture
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is quit message from window or user
	done = false;
	while (!done)
	{
		// Handle windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end application, then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do frame processing
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result;

	// Check if user has pressed Escape
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do frame processing for the application class object
	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}
	
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if key is pressed down on the keyboard
		case WM_KEYDOWN:
		{
			// If a key is pressed, send it to input object so it can record that state
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		
		// Check if key has been released on the keyboard
		case WM_KEYUP:
		{
			// If a key is released, send it to input object so it unset the state for that key
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		
		// Any other messages send to the default message handler as the application won't make use of them
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to handle object
	ApplicationHandle = this;

	// Get the instance of the application
	m_hinstance = GetModuleHandle(NULL);
	
	// Name the Application
	m_applicationName = L"DirectXstuff";

	// Setup the windows class with default settings
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

	// Register the window class
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// Setup the screen settings depending on whether it is running on full screen on windowed mode
	if (FULL_SCREEN)
	{
		// Set screen size to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set position of window to top left corner
		posX = posY = 0;
	}
	else
	{
		// If windowed, set to 800 x 600
		screenWidth = 800;
		screenHeight = 600;
		
		// Place window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings, and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	
	// Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(false);
	
	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the Window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the Application Instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler system class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

