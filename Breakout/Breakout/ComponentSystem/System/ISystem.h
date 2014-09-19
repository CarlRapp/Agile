#ifndef SYSTEM_H
#define SYSTEM_H

#include <algorithm>
#include <vector>
#include "../Entity/Entity.h"
#include "../ClassTypeID.h"
#include "../Filter/ComponentFilter.h"
#include <map>


class ISystem
{
private:
	virtual void Initialize(){}
	virtual void OnEntityAdded(){}
	virtual void OnEntityRemove(){}

	ComponentFilter m_componentFilter;

protected:
	typedef std::map<TypeID, Entity*> EntityMap;
	std::vector<Entity*> m_entities;
	EntityMap m_entityMap;

public:
	ISystem();
	ISystem(const ComponentFilter& _componentFilter);

	virtual ~ISystem() = 0;

	const ComponentFilter* GetComponentFilter() const;
	std::vector<Entity*> GetEntities() const;

	bool Add(Entity* _entity);
	bool Remove(Entity* _entity);
	void Clear(void);

	virtual void Update(float _dt) = 0;

};

template<typename T>
class System
	: public ISystem
{
private:

public:
	typedef System<T> Base;

	System() {}
	System(const ComponentFilter& _componentFilter)
		: ISystem(_componentFilter)
	{
	}

	static TypeID GetTypeID()
	{
		return ClassTypeID<ISystem>::GetTypeId<T>();
	}
};

#endif