#ifndef LOSELIFECOMPONENT_H
#define LOSELIFECOMPONENT_H

#include "IComponent.h"

struct LoseLifeComponent : Component<LoseLifeComponent>
{
public:
	int m_loseLife;

	LoseLifeComponent() : m_loseLife(1) { }
	LoseLifeComponent(int _loseLife) : m_loseLife(_loseLife) { }
};

#endif