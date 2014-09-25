#ifndef DEFLECTIONCOMPONENT_H
#define DEFLECTIONCOMPONENT_H

#include "IComponent.h"

struct DeflectionComponent : Component<DeflectionComponent>
{
	float m_deflectionFactor;

	DeflectionComponent() : m_deflectionFactor(0.0f) { }
	explicit DeflectionComponent(float _deflectionFactor) : m_deflectionFactor(_deflectionFactor) { }
};

#endif