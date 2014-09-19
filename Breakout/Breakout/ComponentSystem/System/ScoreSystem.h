#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "ISystem.h"

class ScoreSystem : public System<ScoreSystem>
{
private:

public:

	ScoreSystem(World* _world);
	~ScoreSystem();

	void Update(float _dt);
};
#endif