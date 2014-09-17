#ifndef SCALECOMPONENT_H
#define SCALECOMPONENT_H

#include "IComponent.h"

struct ScaleComponent : Component<ScaleComponent>
{

	Vector3 m_scale;

	ScaleComponent()
	{
		m_scale = Vector3(1, 1, 1);
	}

};

#endif