#ifndef SHOOTLASERSYSTEM_H
#define SHOOTLASERSYSTEM_H
#include "ISystem.h"
#include <vector>

class ShootLaserSystem : public System<ShootLaserSystem>
{
private:
	const float m_defaultDuration = 10.0f;
	float m_duration;

public:
	ShootLaserSystem(World* _world);
	~ShootLaserSystem();

	void Update(float _dt);
	void ResetDuration();
};



#endif