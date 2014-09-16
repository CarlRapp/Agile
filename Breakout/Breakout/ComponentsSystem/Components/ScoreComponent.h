#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "IComponent.h"

struct ScoreComponent : Component<ScoreComponent>
{
	int m_score;

	ScoreComponent()
	{
		m_score = 0;
	}

};

#endif