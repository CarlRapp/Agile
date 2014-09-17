#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "IComponent.h"

struct PositionComponent : Component<PositionComponent>
{
	Vector3 m_position;
	Vector3 m_deltaPosition;

	PositionComponent()
	{
		m_position = Vector3(0, 0, 0);
		m_deltaPosition = Vector3(0, 0, 0);
	}

	void PositionComponent::Reset()
	{
		m_deltaPosition = Vector3(0, 0, 0);
	}

};

#endif