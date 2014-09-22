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
	if (!m_componentFilter.DoesFilterPass(_entity->GetComponents()))
		return false;

	m_entityMap[_entity->GetId()] = _entity;
	return true;
}

bool ISystem::Remove(Entity* _entity)
{
	if (m_entityMap.find(_entity->GetId()) == m_entityMap.end())
		return false;

	m_entityMap.erase(_entity->GetId());
	return true;
}

void ISystem::Clear(void)
{
	m_entityMap.clear();
}
