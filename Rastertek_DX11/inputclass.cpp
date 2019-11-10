#include "inputclass.h"



InputClass::InputClass()
{
}


InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	// initialize the key state to be released and not pressed
	for ( i = 0; i < 256; i++)
	{
		m_Keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int key)
{
	m_Keys[key] = true;
	return;
}

void InputClass::KeyUp(unsigned int key)
{
	m_Keys[key] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_Keys[key];
}


