#ifndef IINPUT_H
#define IINPUT_H

#pragma region Enums for input
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
#pragma endregion

#pragma region Mouse and Keyboard struct
struct Mouse
{
public:
	Mouse();
	void Update();
	int PositionX;
	int PositionY;
};

#define KEYBOARDKEYS 256
struct Keyboard
{
private:
	bool*	m_lastFrameKeys = new bool[KEYBOARDKEYS];
	bool*	m_thisFrameKeys = new bool[KEYBOARDKEYS];
public:
	Keyboard();
	void Update();
	InputState getKeyState(char _key);
};
#pragma endregion




class IInput
{
protected:
	Mouse* m_mousePointer;
	Keyboard* m_keyboardPointer;
public:
	virtual ~IInput(){};

	virtual void Update() = 0;

	virtual Mouse* getMouse() = 0;
	virtual Keyboard* getKeyboard() = 0;
};

#endif