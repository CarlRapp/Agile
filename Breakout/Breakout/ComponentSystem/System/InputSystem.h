#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "ISystem.h"
#include "../../Input/InputManager.h"

class InputSystem : public System<InputSystem>
{
private:
	Mouse* m_mouse;
	Keyboard* m_keyboard;


public:

	InputSystem(World* _world);
	~InputSystem();

	void Update(float _dt);
};

#endif