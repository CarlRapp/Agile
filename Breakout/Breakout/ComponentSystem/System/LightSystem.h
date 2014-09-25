#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include "ISystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/LightComponent.h"
#include "../../Graphics/GraphicsManager.h"

class LightSystem : public System<LightSystem>
{
private:

	GraphicsManager* m_graphicsManager;
	int m_lightId;

public:

	LightSystem(World* _world);
	~LightSystem();

	bool Add(Entity* _entity);
	void Update(float _dt);
};
#endif