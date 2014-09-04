#ifndef IINPUT_H
#define IINPUT_H

enum InputState
{
	Pressed,
	Down,
	Released,
	Up
};

enum MouseButton
{
	LeftMB,
	RightMB,
	MiddleMB
};

class IInput
{
private:

protected:

public:
	virtual ~IInput(){};

	virtual void Update() = 0;

	virtual InputState getMouseButtonState(MouseButton _button) = 0;
	virtual InputState getKeyState() = 0;
};

#endif