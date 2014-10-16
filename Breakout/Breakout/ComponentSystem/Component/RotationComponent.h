#ifndef ROTATIONCOMPONENT_H
#define ROTATIONCOMPONENT_H

#include "IComponent.h"

struct RotationComponent : Component<RotationComponent>
{
private:
	QUAT m_rotation;
	bool m_changed;

public:
	RotationComponent() : m_rotation(QUAT(0, 0, 0, 0)), m_changed(false) { }
	RotationComponent(QUAT _rotation) : m_rotation(_rotation), m_changed(false) { }

	void Reset()
	{
		m_changed = false;
	}

	QUAT& GetRotation() { return m_rotation; }
	bool HasChanged(){ return m_changed; }
	void SetRotation(const QUAT& _rotation)
        { 
            m_rotation = _rotation; 
            m_changed = true;
        }
};

#endif