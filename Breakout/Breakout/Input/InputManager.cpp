#include "../stdafx.h"

#include "InputManager.h"

#ifdef WINDOWS
#include "Windows/WindowsInput.h"
#else
#include "Linux/LinInput.h"
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
        m_inputDevices = new LinInput();
#endif
}

void InputManager::Update()
{
	if (m_inputDevices)
		m_inputDevices->Update();
}