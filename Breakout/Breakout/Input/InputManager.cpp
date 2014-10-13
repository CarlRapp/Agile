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

	m_imInstance = new InputManager(1280, 720);

	return m_imInstance;
}

InputManager::InputManager(int _screenWidth, int _screenHeight)
{
#ifdef WINDOWS
	m_inputDevices = new WindowsInput(_screenWidth, _screenHeight);
#else
	m_inputDevices = new LinInput(_screenWidth, _screenHeight);
#endif
}

InputManager::~InputManager()
{
	delete m_imInstance;
}

void InputManager::Init(int _screenWidth, int _screenHeight)
{
	if (m_imInstance)
		delete m_imInstance;

	m_imInstance = new InputManager(_screenWidth, _screenHeight);
}

void InputManager::Update()
{
	if (m_inputDevices)
		m_inputDevices->Update();
}