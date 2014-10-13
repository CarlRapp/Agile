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
	MiddleMB = 3
};
#pragma endregion

#pragma region Mouse and Keyboard struct
#define MOUSEBUTTONS 5
struct Mouse
{
private:
	bool*	m_lastFrameButtons = new bool[MOUSEBUTTONS];
	bool*	m_thisFrameButtons = new bool[MOUSEBUTTONS];

	int m_positionX, m_positionY;
	int m_oldPositionX, m_oldPositionY;
	int m_dX, m_dY;

	float m_screenWidth, m_screenHeight;

public:
	Mouse(int _screenWidth, int _screenHeight);
	void Update();

	InputState GetButtonState(char _button);
	float GetX();
	float GetY();
	float GetdX();
	float GetdY();
	void SetMousePosition(float _x, float _y);

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
	InputState GetKeyState(char _key);
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

	virtual Mouse* GetMouse() = 0;
	virtual Keyboard* GetKeyboard() = 0;
};

#endif