#ifndef SYSTEM_H
#define SYSTEM_H

#include <algorithm>
#include <vector>
#include "../Entity/Entity.h"
#include "../ClassTypeID.h"
#include "../Filter/ComponentFilter.h"
#include "../../stdafx.h"
#include <map>

class World;

typedef std::map<TypeID, Entity*> EntityMap;
class ISystem
{
public:
    
    	enum EVENT
	{
		NONE = 0,
		INITIALIZE,
	};
    
private:
	virtual void Initialize(){}
	virtual void OnEntityAdded(Entity* _e){}
	virtual void OnEntityRemoved(Entity* _e){}

	ComponentFilter m_componentFilter;

protected:
	
	EntityMap m_entityMap;
	World* m_world;
    EVENT m_nextEvent;
	virtual void RunEvents(){};



	//Checks if the entity can enter the system by
	//checking its components with the systems filter
	bool CanEnter(Entity* _entity);
public:
    
	ISystem();
	ISystem(const ComponentFilter& _componentFilter, World* _world);

	virtual ~ISystem() = 0;

	const ComponentFilter* GetComponentFilter() const;
	EntityMap GetEntities();

	//Tries to add the entity to the system
	virtual bool Add(Entity* _entity);
	//Tries to remove the entity from the system
	virtual bool Remove(Entity* _entity);
	//If the entity is changed this method is called
	//and rechecks if the entity can still be in the system.
	virtual bool CheckChangedEntity(Entity* _entity);

	//Checks if the entity is in the system
	bool HasEntity(Entity* _e);

	void Clear(void);

	virtual void Update(float _dt) { }
    void SetEvent(EVENT _event){m_nextEvent = _event;}
};

template<typename T>
class System
	: public ISystem
{
private:

public:
	typedef System<T> Base;

	System() {}
	System(const ComponentFilter& _componentFilter, World* _world)
		: ISystem(_componentFilter, _world)
	{
	}

	static TypeID GetTypeID()
	{
		return ClassTypeID<ISystem>::GetTypeId<T>();
	}
};

#endif