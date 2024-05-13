#include "inputclass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	// Initialize all keys to be released and not pressed
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed, save that state in key array
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// If the key is released, clear that state in the key array
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (Pressed or not pressed)
	return m_keys[key];
}