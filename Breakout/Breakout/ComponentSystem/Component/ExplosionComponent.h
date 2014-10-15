#ifndef EXPLOSIONCOMPONENT_H
#define EXPLOSIONCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ExplosionComponent : Component<ExplosionComponent>
{

	enum ExplosionState
	{
		NONE = 0,
		EXPLODING,
		DONE

	};

	float m_explosion = 0.0f;
	float m_maxExplosionTime = .45f;
	float m_expansionRate = 20.0f;
	ExplosionState m_explosionState = NONE;

	ExplosionComponent()	{}

	ExplosionState IsExploding(float _dt)
	{
		if (m_explosionState == EXPLODING)
		{
			m_explosion += _dt * m_expansionRate;

			if (m_explosion > m_maxExplosionTime * m_expansionRate)
			{
				m_explosionState = DONE;
				//printf("Entity done exploding, remove\n");
			}
		}

		return m_explosionState;
	}
};

#endif