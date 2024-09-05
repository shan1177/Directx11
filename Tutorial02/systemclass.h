#ifndef SYSTEMCLASS_H_
#define SYSTEMCLASS_H_

// Pre-processing Directives
#define WIN32_LEAN_AND_MEAN

// Includes
#include <windows.h>

// Class Includes
#include "inputclass.h"
#include "applicationclass.h"

// Class name: SystemClass
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();
	
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// Function Prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals
static SystemClass* ApplicationHandle = 0;

#endif