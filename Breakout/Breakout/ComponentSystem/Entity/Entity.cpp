#include "Entity.h"

int Entity::m_counter = 0;
Entity::Entity()
: m_id(m_counter++), m_state(DEAD), m_components(std::vector<IComponent*>()), m_initialized(false)
{
}

Entity::Entity(int _id)
: m_id(_id), m_state(DEAD), m_components(std::vector<IComponent*>()), m_initialized(false)
{
}

Entity::~Entity()
{
	RemoveAllComponents();
}

void Entity::AddComponent(IComponent* base, TypeID _componentTypeId)
{
	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
		{
			// Log Warning
			return;
		}
	}

	base->m_ID = _componentTypeId;
	m_components.push_back(base);

	m_state = Entity::CHANGED;
}

void Entity::RemoveComponent(int _componentTypeId)
{
	
	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
		{
			//delete m_components[i];
			auto component = m_components[i];
			m_components.erase(m_components.begin() + i);
			delete component;

			m_state = Entity::CHANGED;
			break;
		}
	}

	
}

IComponent* Entity::GetComponent(int _componentTypeId)
{

	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
			return m_components[i];
	}

	return 0;

}

bool Entity::HasComponent(int _componentTypeId)
{

	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
			return true;
	}

	return false;
}

bool Entity::RemoveAllComponents()
{
	for (int i = m_components.size() - 1; i >= 0; --i)
	{
		auto component = m_components[i];
		m_components.erase(m_components.begin() + i);
		delete component;
	}

	return true;
}

std::vector<IComponent*>* Entity::GetComponents()
{
	return &m_components;
}

int Entity::GetId(void)
{
	return m_id;
}

void Entity::Reset(void)
{
	for (int i = 0; i < m_components.size(); ++i)
		m_components[i]->Reset();
}


bool Entity::GetInitialized(void)
{
	return m_initialized;
}
void Entity::SetInitialized(bool _value)
{
	m_initialized = _value;
}