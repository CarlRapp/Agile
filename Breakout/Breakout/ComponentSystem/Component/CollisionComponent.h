#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct CollisionComponent : Component<CollisionComponent>
{
public:
	VECTOR2	m_position, m_size;
	VECTOR2 m_offset;
	CollisionComponent()
	{
		m_position = VECTOR2(0, 0);
		m_offset = VECTOR2(0, 0);
		m_size = VECTOR2(0, 0);
	}
	CollisionComponent(VECTOR2 _pos, VECTOR2 _size)
	{
		m_position = _pos;
		m_offset = VECTOR2(0, 0);
		m_size = _size;
	}

	bool CollidesWith(CollisionComponent* _cc)
	{
		bool a = m_position.x + m_offset.x < _cc->m_position.x + _cc->m_size.x + _cc->m_offset.x;
		bool b = m_position.x + m_size.x + m_offset.x > _cc->m_position.x + _cc->m_offset.x;
		bool c = m_position.y + m_offset.y  < _cc->m_position.y + _cc->m_size.y + _cc->m_offset.y;
		bool d = m_position.y + m_size.y + m_offset.y > _cc->m_position.y + _cc->m_offset.y;
		/*if
			(
			m_position.x < _cc->m_position.x + _cc->m_size.x &&
			m_position.x + m_size.x > _cc->m_position.x &&
			m_position.y < _cc->m_position.y + _cc->m_size.y &&
			m_position.y + m_size.y > _cc->m_position.y
			)*/

		return a && b && c && d;
	}

};


#endif