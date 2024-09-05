#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	return true;
}

void ApplicationClass::Shutdown()
{
	return;
}

bool ApplicationClass::Frame()
{
	return true;
}

bool ApplicationClass::Render()
{
	return true;
}