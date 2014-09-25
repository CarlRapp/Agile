#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "IComponent.h"
#include "../../MathHelper.h"

struct LightComponent : Component<LightComponent>
{
private:
    VECTOR3 m_intensity;
    VECTOR3 m_color;
    float   m_range;

public:
        VECTOR3& GetIntensity()
	{
		return m_intensity;
	}
        
	VECTOR3& GetColor()
	{
		return m_color;
	}

	float& GetRange()
	{
		return m_range;
	}
        
        


	LightComponent()
	{
            m_intensity = VECTOR3(0.0, 0.9, 0.9);
		m_color = VECTOR3(1, 1, 1);
		m_range = 100.f;
	}
};

#endif