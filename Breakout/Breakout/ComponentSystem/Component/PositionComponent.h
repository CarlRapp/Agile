#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "IComponent.h"

struct PositionComponent : Component<PositionComponent>
{
	VECTOR3 m_position;
	VECTOR3 m_deltaPosition;

	PositionComponent()
	{
		m_position = VECTOR3(0, 0, 0);
		m_deltaPosition = VECTOR3(0, 0, 0);
	}

	void PositionComponent::Reset()
	{
		m_deltaPosition = VECTOR3(0, 0, 0);
	}

};

#endif