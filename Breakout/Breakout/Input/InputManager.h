#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "../stdafx.h"
#include "IInput.h"

class InputManager
{
private:
	InputManager();

	static InputManager* m_imInstance;
	IInput* m_inputDevices;

public:
	~InputManager();

	static InputManager* GetInstance();
	
	IInput* getInputDevices()
	{
		return m_inputDevices;
	}
	void Update();
};
#endif