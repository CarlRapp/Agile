#include "InputManager.h"
#include "../stdafx.h"

#ifdef WINDOWS
#include "Windows/WindowsInput.h"
#else

#endif

InputManager* InputManager::m_imInstance = 0;
InputManager* InputManager::GetInstance()
{
	if (m_imInstance)
		return m_imInstance;

	m_imInstance = new InputManager();

	return m_imInstance;
}

InputManager::InputManager()
{
#ifdef WINDOWS
	m_inputDevices = new WindowsInput();
#else
        
#endif
}

void InputManager::Update()
{
	if (m_inputDevices)
		m_inputDevices->Update();
}