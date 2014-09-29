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
	virtual void OnEntityRemove(Entity* _e){}

	ComponentFilter m_componentFilter;

protected:
	
	EntityMap m_entityMap;
	World* m_world;
    EVENT m_nextEvent;
	virtual void RunEvents(){};

public:
    
	ISystem();
	ISystem(const ComponentFilter& _componentFilter, World* _world);

	virtual ~ISystem() = 0;

	const ComponentFilter* GetComponentFilter() const;
	EntityMap GetEntities();

	virtual bool Add(Entity* _entity);
	virtual bool Remove(Entity* _entity);
	void Clear(void);

	virtual void Update(float _dt) = 0;
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