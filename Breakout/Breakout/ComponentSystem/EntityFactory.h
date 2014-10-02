#ifndef ENTITIYFACTORY_H
#define ENTITIYFACTORY_H

#include "Entity/Entity.h"
#include "World.h"

#include "Component/PositionComponent.h"
#include "Component/RotationComponent.h"
#include "Component/ScaleComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ScoreComponent.h"
#include "Component/ModelComponent.h"
#include "Component/LifeComponent.h"
#include "Component/DamageComponent.h"
#include "Component/DeflectionComponent.h"
#include "Component/BounceComponent.h"
#include "Component/MouseInputComponent.h"
#include "Component/KeyboardInputComponent.h"
#include "Component/AudioComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/LightComponent.h"
#include "System/PhysicsSystem.h"
#include "Component/PlayerLifeComponent.h"
#include "Component/EffectComponent.h"
#include "Component/ShatterComponent.h"
#include "Component/BlockComponent.h"

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
		WALL,
		H_WALL,
		INVISIBLE_WALL,
		PROJECTILE,
		PLAYER,
		POINTLIGHT,
		PLANE
	};

	static EntityFactory* GetInstance();

	// Creates entity and required components based on EntityType
	void CreateEntity(Entity* _entity, EntityType _entityType);


	void CreateBlockField(World* _world);


};




#endif

