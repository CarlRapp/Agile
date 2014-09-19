#ifndef SCALECOMPONENT_H
#define SCALECOMPONENT_H

#include "IComponent.h"

struct ScaleComponent : Component<ScaleComponent>
{

	VECTOR3 m_scale;
	VECTOR3 m_deltaScale;

	ScaleComponent()
	{
		m_scale = VECTOR3(1, 1, 1);
		m_deltaScale = VECTOR3(0, 0, 0);
	}

	void ScaleComponent::Reset()
	{
		m_deltaScale = VECTOR3(0, 0, 0);
	}

};

#endif