#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "IComponent.h"

struct ScoreComponent : Component<ScoreComponent>
{
	int m_score=0;
	bool wasHit=false; // temp
        std::string m_scoreStr = "SCORE :";
        int m_nextLevel = 5;

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
        
        void AddScore(int _add)
        {
            m_score += _add;
        }
        
        int CheckLevelUp()
        {
            int levelUp = 1;
            int levels = 0;
            
            while(levelUp > 0)
            {
                levelUp = m_score - m_nextLevel;
                
                if(levelUp > 0)
                {
                    levels++;
                    SetNextLevelUpScore();
                }
            }
            
            return levels;
        }
        
        void SetNextLevelUpScore()
        {
            m_nextLevel += 5000;
        }

};

#endif