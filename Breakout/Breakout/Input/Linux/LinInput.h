#ifndef LININPUT_H
#define	LININPUT_H

#include "../IInput.h"


class LinInput : public IInput
{
public:
	LinInput(int _screenWidth, int _screenHeight);
    ~LinInput();
    void Update();
    Mouse* GetMouse();
    Keyboard* GetKeyboard();
};

#endif


