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

	void SetIntensity(VECTOR3 _intensity)
	{
		m_intensity = _intensity;
	}

	void SetColor(VECTOR3 _color)
	{
		m_color = _color;
	}

	void SetRange(float _range)
	{
		m_range = _range;
	}


	LightComponent()
	{
                m_intensity = VECTOR3(0.2, 0.8, 1.0);
		m_color = VECTOR3(1, 1, 1);
		m_range = 10.f;
	}
};

#endif