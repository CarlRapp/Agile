#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionComponent : Component<CollisionComponent>
{
public:
	VECTOR2	m_position, m_size;
	CollisionComponent()
	{
		m_position = VECTOR2(0, 0);
		m_size = VECTOR2(0, 0);
	}
	CollisionComponent(VECTOR2 _pos, VECTOR2 _size)
	{
		m_position = _pos;
		m_size = _size;
	}

	bool CollidesWith(VECTOR2 _position, VECTOR2 _size)
	{
		if
			(
			m_position.x < _position.x + _size.x &&
			m_position.x + m_size.x > _position.x &&
			m_position.y < _position.y + _size.y &&
			m_position.y + m_size.y > _position.y
			)
			return true;

		return false;
	}

};


#endif