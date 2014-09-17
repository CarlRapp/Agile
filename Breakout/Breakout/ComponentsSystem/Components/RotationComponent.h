#ifndef ROTATIONCOMPONENT_H
#define ROTATIONCOMPONENT_H

#include "IComponent.h"

struct RotationComponent : Component<RotationComponent>
{
	Vector3 m_rotation;

	RotationComponent()
	{
		m_rotation = Vector3(0, 0, 0);
	}

};

#endif