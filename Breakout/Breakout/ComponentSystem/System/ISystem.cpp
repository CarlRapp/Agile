#include "ISystem.h"

ISystem::ISystem()
{
}

ISystem::ISystem(const ComponentFilter& _componentFilter)
: m_componentFilter(_componentFilter), m_entityMap(EntityMap())
{
}

ISystem::~ISystem()
{
}

std::vector<Entity*> ISystem::GetEntities() const
{
	return m_entities;
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
	m_entities.push_back(_entity);
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
	m_entities.clear();
}