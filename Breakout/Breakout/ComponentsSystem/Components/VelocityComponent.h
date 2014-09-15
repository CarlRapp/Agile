#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "IComponent.h"

struct VelocityComponent : Component<VelocityComponent>
{
	float m_X;
	float m_Y;
	float m_Z;

	VelocityComponent()
	{
		m_X = 0;
		m_Y = 0;
		m_Z = 0;
	}

};

#endif