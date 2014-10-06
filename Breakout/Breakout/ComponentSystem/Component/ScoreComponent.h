#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "IComponent.h"

struct ScoreComponent : Component<ScoreComponent>
{
	int m_score;
	bool wasHit; // temp
        std::string m_scoreStr;

	ScoreComponent()
	{
		m_score = 0;
		wasHit = false;
	}
        
        void SetString()
        {
            m_scoreStr = "SCORE: "+std::to_string(m_score);
        }
        
        std::string* GetString()
        {
            return &m_scoreStr;
        }

};

#endif