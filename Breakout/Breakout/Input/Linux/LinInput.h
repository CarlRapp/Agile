#ifndef LININPUT_H
#define	LININPUT_H

#include "../IInput.h"


class LinInput : public IInput
{
public:
    LinInput();
    ~LinInput();
    void Update();
    Mouse* GetMouse();
    Keyboard* GetKeyboard();
};

#endif


