#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "IInput.h"

class InputManager
{
private:
	InputManager();

	static InputManager* m_imInstance;
	IInput* m_inputHandle;

public:
	~InputManager();

	static InputManager* GetInstance();
	
	IInput* getInputHandle()
	{
		return m_inputHandle;
	}
	void Update();
};
#endif