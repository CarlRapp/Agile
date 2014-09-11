#include "ISystem.h"

ISystem::ISystem()
{
}

ISystem::ISystem(const ComponentFilter& _componentFilter)
: m_componentFilter(_componentFilter)
{
}

ISystem::~ISystem()
{
}

std::vector<Entity*> ISystem::GetEntities() const
{
	return m_entities;
}

const ComponentFilter& ISystem::GetComponentFilter() const
{
	return m_componentFilter;
}

bool ISystem::Add(Entity* _entity)
{
	int newId = _entity->GetId();


	for (int i = 0; i < m_entities.size(); ++i)
	{
		if (newId == m_entities[i]->GetId())
		{
			//Log Warning
			return false;
		}
	}

	m_entities.push_back(_entity);
	return true;
}

bool ISystem::Remove(Entity* _entity)
{
	if (m_entities.size() == 0)
	{
		// Log warning
		return true;
	}


	auto iter = std::find(m_entities.begin(), m_entities.end(), _entity);
	if (iter != m_entities.end())
	{
		m_entities.erase(iter);
		return true;
	}

	// Log Warning
	return false;
}

void ISystem::Clear(void)
{
	m_entities.clear();
}