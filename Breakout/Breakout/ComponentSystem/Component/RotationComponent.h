#ifndef ROTATIONCOMPONENT_H
#define ROTATIONCOMPONENT_H

#include "IComponent.h"

struct RotationComponent : Component<RotationComponent>
{
	VECTOR3 m_rotation;
	VECTOR3 m_deltaRotation;

	RotationComponent()
	{
		m_rotation = VECTOR3(0, 0, 0);
		m_deltaRotation = VECTOR3(1, 0, 0);
	}

	void Reset()
	{
		m_deltaRotation = VECTOR3(0, 0, 0);
	}

};

#endif