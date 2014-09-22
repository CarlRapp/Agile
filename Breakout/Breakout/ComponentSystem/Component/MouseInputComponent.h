#ifndef MOUSEINPUTCOMPONENT_H
#define MOUSEINPUTCOMPONENT_H

#include "IComponent.h"
#include "../../Input/IInput.h"

struct MouseInputComponent : Component<MouseInputComponent>
{
	struct Controls
	{

		Controls()
		:
		LeftButton(InputState::Up),
		RightButton(InputState::Up),
		MiddleButton(InputState::Up),
		MouseDX(0),
		MouseDY(0)
		{
		}

		InputState LeftButton;
		InputState RightButton;
		InputState MiddleButton;

		int MouseDX;
		int MouseDY;

	} m_controls;
	MouseInputComponent() : m_controls()
	{
	}

};

#endif