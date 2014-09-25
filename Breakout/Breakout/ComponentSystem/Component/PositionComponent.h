#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "IComponent.h"

struct PositionComponent : Component<PositionComponent>
{
private:
	VECTOR3 m_position;
	VECTOR3 m_deltaPosition;

public:
	PositionComponent() : m_position(VECTOR3(0, 0, 0)), m_deltaPosition(VECTOR3(0, 0, 0)) { }	
	PositionComponent(VECTOR3 _position) : m_position(_position), m_deltaPosition(VECTOR3(0, 0, 0)) { }

	void Reset()
	{
		m_deltaPosition = VECTOR3(0, 0, 0);
	}

	VECTOR3& GetPosition() { return m_position; }
	const VECTOR3& GetDeltaPosition() const { return m_deltaPosition; }
	void SetPosition(const VECTOR3& _position) { m_deltaPosition = VECTOR3(_position.x - m_position.x, _position.y - m_position.y, _position.z - m_position.z); m_position = _position; }
};

#endif