#ifndef SHATTERCOMPONENT_H
#define SHATTERCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ShatterComponent : Component<ShatterComponent>
{

	enum ShatterState
	{
		NONE = 0,
		SHATTERING,
		DONE
	};

	float m_explosion = 0.0f;
	float m_maxExplosionTime = 0.2f;
	float m_expansionRate = 20.0f;
	ShatterState m_explosionState = NONE;

	ShatterComponent()	{}

	ShatterState IsShattering(float _dt)
	{
		if (m_explosionState == SHATTERING)
		{
			m_explosion += _dt * m_expansionRate;

			if (m_explosion > m_maxExplosionTime * m_expansionRate)
			{
				m_explosionState = DONE;
				printf("Entity done shattering, remove\n");
			}
		}

		return m_explosionState;
	}
};

#endif