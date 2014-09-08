#include "InputManager.h"
#include "Windows/WindowsInput.h"

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
	/*	TODO add #ifndef Linux / windows	*/
	m_inputDevices = new WindowsInput();
}

void InputManager::Update()
{
	if (m_inputDevices)
		m_inputDevices->Update();
}