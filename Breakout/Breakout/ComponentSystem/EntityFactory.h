#ifndef ENTITIYFACTORY_H
#define ENTITIYFACTORY_H

#include "Entity/Entity.h"


#include "Component/PositionComponent.h"
#include "Component/RotationComponent.h"
#include "Component/ScaleComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ScoreComponent.h"
#include "Component/ModelComponent.h"
#include "Component/LifeComponent.h"
#include "Component/MouseInputComponent.h"
#include "Component/KeyboardInputComponent.h"
#include "System/PhysicsSystem.h"


class EntityFactory
{
private:
	EntityFactory(void) {};

	static EntityFactory* m_entityFactory;

public:
	~EntityFactory(void);

	enum EntityType
	{
		BLOCK = 0,
		PAD,
		BALL,
		POWERUP,
		WALL
	};

	static EntityFactory* GetInstance();

	// Creates entity and required components based on EntityType
	void CreateEntity(Entity* _entity, EntityType _entityType);


};




#endif

