#ifndef SCALECOMPONENT_H
#define SCALECOMPONENT_H

#include "IComponent.h"

struct ScaleComponent : Component<ScaleComponent>
{

	Vector3 m_scale;
	Vector3 m_deltaScale;

	ScaleComponent()
	{
		m_scale = Vector3(1, 1, 1);
		m_deltaScale = Vector3(0, 0, 0);
	}

	void ScaleComponent::Reset()
	{
		m_deltaScale = Vector3(0, 0, 0);
	}

};

#endif