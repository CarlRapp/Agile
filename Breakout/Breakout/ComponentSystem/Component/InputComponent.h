#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "IComponent.h"
#include "../../Input/IInput.h"

struct InputComponent : Component<InputComponent>
{
	struct Controls
	{

		Controls()
		:
		LeftButton(MouseButton::LeftMB),
		MouseDX(0),
		MouseDY(0)
		{
		}

		int LeftButton;

		int MouseDX;
		int MouseDY;
	} m_controls;
	InputComponent() : m_controls()
	{
	}

};

#endif