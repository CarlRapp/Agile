#ifndef VELOCITYCOMPONENT_H
#define VELOCITYCOMPONENT_H

#include "IComponent.h"
#include "../../stdafx.h"

struct VelocityComponent : Component<VelocityComponent>
{
	VECTOR3 m_velocity;

	VelocityComponent()
	{
		m_velocity = VECTOR3(1, 5, 0);
	}

};

#endif