#include <stdio.h>
#include <SDL.h>
#include <Windows.h>
#include "Input/IInput.h"


#define KEYBOARDKEYS 256
bool*	m_lastFrameKeys = new bool[KEYBOARDKEYS];
bool*	m_thisFrameKeys = new bool[KEYBOARDKEYS];

InputState getKeyState(char Key)
{
	bool	tLastFrame = m_lastFrameKeys[Key];
	bool	tThisFrame = m_thisFrameKeys[Key];

	if (!tLastFrame && tThisFrame)
		return InputState::Pressed;

	else if (tLastFrame && tThisFrame)
		return InputState::Down;

	else if (tLastFrame && !tThisFrame)
		return InputState::Released;


	return InputState::Up;
}

int main(int argc, char** argv)
{


	while (true)
	{
		for (int n = 0; n < KEYBOARDKEYS; ++n)
		{
			m_lastFrameKeys[n] = m_thisFrameKeys[n];
			m_thisFrameKeys[n] = GetAsyncKeyState(n) ? true : false;
		}


		if (getKeyState('A') == InputState::Pressed)
			printf("First click\n");
		if (getKeyState('A') == InputState::Down)
			printf("Still down\n");
		if (getKeyState('A') == InputState::Released)
			printf("Released\n");


		//SDL_Delay(100);
	}

	return 0;
}