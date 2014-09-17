#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "IComponent.h"

struct PositionComponent : Component<PositionComponent>
{
	Vector3 m_position;

	PositionComponent()
	{
		m_position = Vector3(0, 0, 0);
	}

};

#endif