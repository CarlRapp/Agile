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
#include "Component/CollisionStatsComponent.h"
#include "Component/DeflectionComponent.h"
#include "Component/MouseInputComponent.h"
#include "Component/KeyboardInputComponent.h"
#include "Component/AudioComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/LightComponent.h"
#include "Component/LoseLifeComponent.h"
#include "Component/SpawnEntityComponent.h"
#include "System/PhysicsSystem.h"
#include "Component/EffectComponent.h"
#include "Component/ShatterComponent.h"
#include "Component/BlockComponent.h"
#include "Component/TextComponent.h"
#include "Component/ExplosionComponent.h"
#include "Component/TNTComponent.h"

class EntityFactory
{
private:
	EntityFactory(void) {};

	static EntityFactory* m_entityFactory;

public:
	~EntityFactory(void);

	enum EntityType
	{
		STANDARD_BLOCK_RED = 0,
		STANDARD_BLOCK_BLUE,
		STANDARD_BLOCK_GREEN,
		INDESTRUCTIBLE_BLOCK,
		TNT_BLOCK,
		PAD,
		SAUSAGE_PAD_MID,
		SAUSAGE_PAD_EDGE,
		BALL,
		POWERUP,
		WALL,
		H_WALL,
		INVISIBLE_WALL,
		PROJECTILE,
		PLAYER,
		POINTLIGHT,
		PLANE,
        TEXT,
		EXPLOSION,
		SHATTER,
		STANDARD_HORIZONTAL_RECTANGLE
	};

	static EntityFactory* GetInstance();

	// Creates entity and required components based on EntityType
	void CreateEntity(Entity* _entity, EntityType _entityType);


	void CreateBlockField(World* _world);


};




#endif

