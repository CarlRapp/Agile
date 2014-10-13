#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "../stdafx.h"
#include "IInput.h"

class InputManager
{
private:
	InputManager(int _screenWidth, int _screenHeight);

	static InputManager* m_imInstance;
	IInput* m_inputDevices;

public:
	~InputManager();

	static InputManager* GetInstance();
	static void Init(int _screenWidth, int _screenHeight);
	
	IInput* getInputDevices()
	{
		return m_inputDevices;
	}
	void Update();
};
#endif