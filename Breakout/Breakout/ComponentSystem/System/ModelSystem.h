#ifndef MODELSYSTEM_H
#define MODELSYSTEM_H

#include "ISystem.h"

class ModelSystem : public System<ModelSystem>
{
private:
public:

	ModelSystem(World* _world);
	~ModelSystem();

	void Update(float _dt);
};

#endif