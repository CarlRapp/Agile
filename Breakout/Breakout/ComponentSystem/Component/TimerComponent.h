#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct TimerComponent : Component<TimerComponent>
{

	enum TimerState
	{
		NONE = 0,
		TICKING,
		DONE
	};

	float m_currentTime = 0.0f;
	float m_maxTime = 2.f;
	float m_tickRate = 4.0f;
	TimerState m_timerState = NONE;

	TimerComponent()	{}

	TimerState IsTicking(float _dt)
	{
		if (m_timerState == TICKING)
		{
			m_currentTime += _dt * m_tickRate;

			if (m_currentTime > m_maxTime * m_tickRate)
			{
				m_timerState = DONE;
				//m_currentTime = m_maxTime;
				//printf("Entity done shattering, remove\n");
			}
		}

		return m_timerState;
	}
};

#endif