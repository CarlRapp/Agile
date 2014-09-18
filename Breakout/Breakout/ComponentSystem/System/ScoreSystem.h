#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "ISystem.h"

class ScoreSystem : public System<ScoreSystem>
{
private:

public:

	ScoreSystem();
	~ScoreSystem();

	void Update(float _dt);
};
#endif