#ifndef KEYBOARDINPUTCOMPONENT_H
#define KEYBOARDINPUTCOMPONENT_H

#include "IComponent.h"
#include "../../Input/IInput.h"
#include <map>

struct KeyboardInputComponent : Component<KeyboardInputComponent>
{
	std::map<int, InputState> m_keys;

};

#endif