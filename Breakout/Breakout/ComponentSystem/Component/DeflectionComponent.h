#ifndef DEFLECTIONCOMPONENT_H
#define DEFLECTIONCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct DeflectionComponent : Component<DeflectionComponent>
{
private:
	float m_deflection;
public:
	DeflectionComponent() : m_deflection(0.0f) { }
	DeflectionComponent(float _deflection) : m_deflection(_deflection) {}
	~DeflectionComponent() { }

	float GetDeflection() const { return m_deflection; }
	void SetDeflection(const float _deflection) { m_deflection = _deflection; }
};

#endif
