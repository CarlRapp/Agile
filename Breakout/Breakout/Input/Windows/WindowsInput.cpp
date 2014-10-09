#include "WindowsInput.h"
#include <Windows.h>
#include "../../Graphics/DirectX/DXGraphics.h"
#include "../../Graphics/GraphicsManager.h"
#include <stdio.h>


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
	if (m_mousePointer != 0 && m_keyboardPointer != 0 && GetActiveWindow() == ((DXGraphics*)(GraphicsManager::GetInstance()->GetIGraphics()))->GetWindow()->GetHandle())
	{
		m_mousePointer->Update();
		m_keyboardPointer->Update();
	}
}



Mouse* WindowsInput::GetMouse()
{
	return m_mousePointer;
}

Keyboard* WindowsInput::GetKeyboard()
{
	return m_keyboardPointer;
}

/*
	Windows Specific implementation
	of Mouse and Keyboard
*/
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

InputState Keyboard::GetKeyState(char _key)
{
	_key = toupper(_key);
	if (_key < KEYBOARDKEYS)
	{
		bool	tLastFrame = m_lastFrameKeys[_key];
		bool	tThisFrame = m_thisFrameKeys[_key];

		if (!tLastFrame && tThisFrame)
			return InputState::Pressed;

		else if (tLastFrame && tThisFrame)
			return InputState::Down;

		else if (tLastFrame && !tThisFrame)
			return InputState::Released;

	}
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

	m_positionX = 0;
	m_positionY = 0;
	m_oldPositionX = 0;
	m_oldPositionY = 0;
	m_dX = 0;
	m_dY = 0;
}
void Mouse::Update()
{
	for (int n = 0; n < MOUSEBUTTONS; ++n)
	{
		m_lastFrameButtons[n] = m_thisFrameButtons[n];
		m_thisFrameButtons[n] = GetAsyncKeyState(n) ? true : false;
	}

	POINT A;

	//	Calculate the position of the mouse
	//	relative to the window of the game
	GetCursorPos(&A);
	//ScreenToClient(
	//	((DXGraphics*)(GraphicsManager::GetInstance()->GetIGraphics()))->GetWindow()->GetHandle(),
	//	&A
	//	);

	m_positionX = A.x;
	m_positionY = A.y;
	m_dX = m_positionX - m_oldPositionX;
	m_dY = m_oldPositionY - m_positionY;
	m_oldPositionX = m_positionX;// = A.x;
	m_oldPositionY = m_positionY;// = A.y;
		
}
int Mouse::GetdX() { return m_dX; }
int Mouse::GetdY() { return m_dY; }
int Mouse::GetX() { return m_positionX; }
int Mouse::GetY() { return m_positionY; }
void Mouse::SetMousePosition(int _x, int _y)
{
	SetCursorPos(_x, _y);
	m_positionX = _x;
	m_positionY = _y;
	m_oldPositionX = _x;
	m_oldPositionY = _y;
}

InputState Mouse::GetButtonState(char _button)
{
	_button++;
	if (_button < MOUSEBUTTONS)
	{
		bool	tLastFrame = m_lastFrameButtons[_button];
		bool	tThisFrame = m_thisFrameButtons[_button];

		if (!tLastFrame && tThisFrame)
			return InputState::Pressed;

		else if (tLastFrame && tThisFrame)
			return InputState::Down;

		else if (tLastFrame && !tThisFrame)
			return InputState::Released;
	}

	return InputState::Up;
}
#pragma endregion
