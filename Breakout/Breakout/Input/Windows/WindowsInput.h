#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H
#include "../IInput.h"


class WindowsInput : public IInput
{

public:
	WindowsInput(int _screenWidth, int _screenHeight);
	~WindowsInput();

	void Update();
	Mouse* GetMouse();
	Keyboard* GetKeyboard();
};


#endif