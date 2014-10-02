#include "ISystem.h"

ISystem::ISystem()
{
}

ISystem::ISystem(const ComponentFilter& _componentFilter, World* _world)
: m_componentFilter(_componentFilter), m_entityMap(EntityMap()), m_world(_world)
{

}

ISystem::~ISystem()
{
}

EntityMap ISystem::GetEntities()
{
	return m_entityMap;
}

const ComponentFilter* ISystem::GetComponentFilter() const
{
	return &m_componentFilter;
}

bool ISystem::Add(Entity* _entity)
{
	if (m_entityMap.find(_entity->GetId()) != m_entityMap.end())
		return false;

	//	Check filter
	if (!CanEnter(_entity))
		return false;

	m_entityMap[_entity->GetId()] = _entity;
	OnEntityAdded(_entity);
	return true;
}

bool ISystem::Remove(Entity* _entity)
{
	if (m_entityMap.find(_entity->GetId()) == m_entityMap.end())
		return false;

	m_entityMap.erase(_entity->GetId());
	OnEntityRemoved(_entity);
	return true;
}

bool ISystem::HasEntity(Entity* _e)
{
	return m_entityMap.find(_e->GetId()) != m_entityMap.end();
}

bool ISystem::CheckChangedEntity(Entity* _entity)
{
	if (!HasEntity(_entity))
		return false;


	return CanEnter(_entity);
}

bool ISystem::CanEnter(Entity* _entity)
{
	return m_componentFilter.DoesFilterPass(_entity->GetComponents());
}

void ISystem::Clear(void)
{
	m_entityMap.clear();
}
