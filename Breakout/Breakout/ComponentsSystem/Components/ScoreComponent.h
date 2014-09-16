#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "IComponent.h"

struct ScoreComponent : Component<ScoreComponent>
{
	int m_score;
	bool wasHit; // temp

	ScoreComponent()
	{
		m_score = 1;
		wasHit = false;
	}

};

#endif