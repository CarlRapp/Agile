#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H
#include "../IInput.h"


class WindowsInput : public IInput
{
private:

public:
	WindowsInput();
	~WindowsInput();

	void Update();
	Mouse* getMouse();
	Keyboard* getKeyboard();
};


#endif