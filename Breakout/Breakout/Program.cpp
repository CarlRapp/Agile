#include <stdio.h>
#include <SDL.h>
#include <Windows.h>
#include "Input/Windows/WindowsInput.h"
#include "Input/InputManager.h"

int main(int argc, char** argv)
{
	InputManager* IM = InputManager::GetInstance();
	IInput* INPUT = IM->getInputHandle();

	Keyboard* KEYBOARD = INPUT->getKeyboard();
	while (true)
	{
		IM->Update();
		if (KEYBOARD)
		{
			char i = VK_SHIFT;
			if (KEYBOARD->getKeyState(5) == InputState::Pressed)
				printf("%c was just pressed!\n", (char)i);
			if (KEYBOARD->getKeyState(5) == InputState::Down)
				printf("%c is down!\n", (char)i);
			if (KEYBOARD->getKeyState(5) == InputState::Released)
				printf("%c is released!\n", (char)i);

		}

		POINT p;
		GetCursorPos(&p);

		printf("X: %d\nY:%d\n", p.x, p.y);



		SDL_Delay(100);
	}

	return 0;
}