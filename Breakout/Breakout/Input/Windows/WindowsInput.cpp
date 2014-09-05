#include "WindowsInput.h"
#include <Windows.h>

#pragma region Keyboard implementation
Keyboard::Keyboard()
{
	for (int n = 0; n < KEYBOARDKEYS; ++n)
	{
		m_lastFrameKeys[n] = 0;
		m_thisFrameKeys[n] = 0;
	}
}

void Keyboard::Update()
{
	for (int n = 0; n < KEYBOARDKEYS; ++n)
	{
		m_lastFrameKeys[n] = m_thisFrameKeys[n];
		m_thisFrameKeys[n] = GetAsyncKeyState(n) ? true : false;
	}
}

InputState Keyboard::getKeyState(char _key)
{
	bool	tLastFrame = m_lastFrameKeys[_key];
	bool	tThisFrame = m_thisFrameKeys[_key];

	if (!tLastFrame && tThisFrame)
		return InputState::Pressed;

	else if (tLastFrame && tThisFrame)
		return InputState::Down;

	else if (tLastFrame && !tThisFrame)
		return InputState::Released;


	return InputState::Up;
}
#pragma endregion

#pragma region Mouse implementation
Mouse::Mouse()
{
	for (int n = 0; n < MOUSEBUTTONS; ++n)
	{
		m_lastFrameButtons[n] = 0;
		m_thisFrameButtons[n] = 0;
	}

	absoluteX = 0;
	absoluteY = 0;
	relativeX = 0;
	relativeY = 0;
}
void Mouse::Update()
{
	for (int n = 0; n < MOUSEBUTTONS; ++n)
	{
		m_lastFrameButtons[n] = m_thisFrameButtons[n];
		m_thisFrameButtons[n] = GetAsyncKeyState(n) ? true : false;
	}
}
#pragma endregion


WindowsInput::WindowsInput()
{
	m_mousePointer = new Mouse();
	m_keyboardPointer = new Keyboard();
}

WindowsInput::~WindowsInput()
{
	delete m_mousePointer;
	delete m_keyboardPointer;
}

void WindowsInput::Update()
{
	if (m_mousePointer != 0 && m_keyboardPointer != 0)
	{
		m_mousePointer->Update();
		m_keyboardPointer->Update();
	}
}



Mouse* WindowsInput::getMouse()
{
	return m_mousePointer;
}

Keyboard* WindowsInput::getKeyboard()
{
	return m_keyboardPointer;
}