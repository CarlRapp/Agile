#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "ISystem.h"

class ScoreSystem : public System<ScoreSystem>
{
private:
	int m_frameScore;
	void OnEntityAdded(Entity* _e);
	void OnEntityRemoved(Entity* _e);
        int m_levelUp = 0;
        std::string ExpToString(int _nextLevel, int _score,int _level, float _increase);

public:

	ScoreSystem(World* _world);
	~ScoreSystem();

	void Update(float _dt);
};

#endif