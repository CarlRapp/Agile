#ifndef LIFECOMPONENT_H
#define LIFECOMPONENT_H

#include "IComponent.h"

struct LifeComponent : Component<LifeComponent>
{
	int m_maxNoLifes;
	int m_noLifes;

	std::string m_lifeStr;

	LifeComponent()
	{
		m_maxNoLifes = 10;
		m_noLifes = 3;
	}

	void SetString()
	{
		m_lifeStr = "LIFE: " + std::to_string(m_noLifes);
	}

	std::string* GetString()
	{
		return &m_lifeStr;
	}

};

#endif