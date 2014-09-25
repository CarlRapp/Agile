#ifndef BOUNCECOMPONENT_H
#define BOUNCECOMPONENT_H

#include "IComponent.h"

struct BounceComponent : Component<BounceComponent>
{
	float m_deflectionFactor;

	BounceComponent() : m_deflectionFactor(0.0f) { }
	explicit BounceComponent(float _deflectionFactor) : m_deflectionFactor(_deflectionFactor) { }
};

#endif