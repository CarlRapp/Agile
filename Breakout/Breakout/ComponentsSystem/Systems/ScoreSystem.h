#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "ISystem.h"

class ScoreSystem : public System<ScoreSystem>
{
private:

	Entity* m_player;

public:

	ScoreSystem();
	void AddPlayer(Entity* _player);
	~ScoreSystem();

	void Update(double _dt);
};
#endif