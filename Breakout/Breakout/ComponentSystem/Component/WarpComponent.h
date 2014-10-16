#ifndef WARPCOMPONENT_H
#define WARPCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct WarpComponent : Component<WarpComponent>
{
	enum WarpType
	{
		MIN_TO_MAX,
		MAX_TO_MIN
	};

	enum WarpState
	{
		NONE = 0,
		FIRST_WARP,
		SECOND_WARP,
		DONE
	};

	WarpState m_warpState = NONE;
	WarpType m_type = MIN_TO_MAX;

	VECTOR3 m_newScale = VECTOR3(0,0,0);
	float m_multiply = 20;
	float m_maxSize = 3.f;
	float m_finishedSize = 2.f;

	WarpComponent()	{}

	void Warping(float _dt)
	{
		WarpMinToMax(_dt);
	}
private:
	void WarpMinToMax(float _dt)
	{
		if (m_warpState == FIRST_WARP)
		{
			if (m_newScale.x < m_maxSize)
			{
				m_newScale.x += _dt * m_multiply;
				m_newScale.y += _dt * m_multiply;
				m_newScale.z += _dt * m_multiply;

				m_newScale.x = m_newScale.x > m_maxSize ? m_maxSize : m_newScale.x;
				m_newScale.y = m_newScale.y > m_maxSize ? m_maxSize : m_newScale.y;
				m_newScale.z = m_newScale.z > m_maxSize ? m_maxSize : m_newScale.z;
			}
			else
				m_warpState = SECOND_WARP;
		}
		else if (m_warpState == SECOND_WARP)
		{
			if (m_newScale.x > m_finishedSize)
			{
				m_newScale.x -= _dt * m_multiply * 0.5f;
				m_newScale.y -= _dt * m_multiply * 0.5f;
				m_newScale.z -= _dt * m_multiply * 0.5f;

				m_newScale.x = m_newScale.x < m_finishedSize ? m_finishedSize : m_newScale.x;
				m_newScale.y = m_newScale.y < m_finishedSize ? m_finishedSize : m_newScale.y;
				m_newScale.z = m_newScale.z < m_finishedSize ? m_finishedSize : m_newScale.z;
			}
			else
				m_warpState = DONE;
		}
	}

	void WarpMaxToMin(float _dt)
	{

	}
};

#endif