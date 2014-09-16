#ifndef ENTITIYFACTORY_H
#define ENTITIYFACTORY_H

#define MAX_ENTITY_COUNT 500

#include "Entity\Entity.h"

class EntityFactory
{
private:
	EntityFactory(void) {};

	static EntityFactory* m_entityFactory;
	static bool m_initialized;


	Entity** m_entites;
	

public:

	enum EntityType
	{
		BLOCK = 0,
		PAD,
		BALL,
		POWERUP,
		PLAYER
	};



	~EntityFactory(void);
	
	static EntityFactory* GetInstance();

	// Creates entity and required components based on EntityType
	Entity* CreateEntity(EntityType _entityType);
	// Will clear the entity from all components and change its status to 'DEAD'
	void DeleteEntity(Entity* _entity);

	// Must be called before any other function is called
	void Initialize(Entity** _entites);

};
#endif