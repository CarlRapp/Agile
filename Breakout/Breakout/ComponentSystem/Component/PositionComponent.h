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
	PositionComponent(VECTOR3 _position) : m_position(_position)
	{
		m_deltaPosition = VECTOR3(1, 0, 0);
	}

	void Reset()
	{
		m_deltaPosition = VECTOR3(0, 0, 0);
	}

};

#endif