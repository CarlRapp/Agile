#ifndef ROTATIONCOMPONENT_H
#define ROTATIONCOMPONENT_H

#include "IComponent.h"

struct RotationComponent : Component<RotationComponent>
{
private:
	VECTOR3 m_rotation;
	VECTOR3 m_deltaRotation;

public:
	RotationComponent() : m_rotation(VECTOR3(0, 0, 0)), m_deltaRotation(VECTOR3(0, 0, 0)) { }
	RotationComponent(VECTOR3 _rotation) : m_rotation(_rotation), m_deltaRotation(VECTOR3(0, 0, 0)) { }

	void Reset()
	{
		m_deltaRotation = VECTOR3(0, 0, 0);
	}

	VECTOR3& GetRotation() { return m_rotation; }
	const VECTOR3& GetDeltaRotation() const { return m_deltaRotation; }
	void SetRotation(const VECTOR3& _rotation) { m_deltaRotation = VECTOR3(_rotation.x - m_rotation.x, _rotation.y - m_rotation.y, _rotation.z - m_rotation.z); m_rotation = _rotation; }
};

#endif