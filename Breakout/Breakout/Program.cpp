#include <stdio.h>
#include <SDL.h>
#include <Windows.h>
#include "Input/Windows/WindowsInput.h"


int main(int argc, char** argv)
{
	WindowsInput* INPUT = new WindowsInput();

	Keyboard* KEYBOARD = INPUT->getKeyboard();
	while (true)
	{
		INPUT->Update();
		if (KEYBOARD)
		{
			char i = VK_SHIFT;
			if (KEYBOARD->getKeyState((char)i) == InputState::Pressed)
				printf("%c was just pressed!\n", (char)i);
			if (KEYBOARD->getKeyState((char)i) == InputState::Down)
				printf("%c is down!\n", (char)i);
			if (KEYBOARD->getKeyState((char)i) == InputState::Released)
				printf("%c is released!\n", (char)i);

		}



		SDL_Delay(100);
	}

	return 0;
}