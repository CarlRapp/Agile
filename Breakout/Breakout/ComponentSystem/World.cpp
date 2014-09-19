#include "World.h"

World::World()
{
	Start();
}

World::~World()
{

}

void World::Start()
{
	/*	Allocate all entities	*/
	m_entityPool = new Entity*[MAX_ENTITY_COUNT];
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
		m_entityPool[i] = new Entity(i);

	printf("%d Entities allocated!\n", MAX_ENTITY_COUNT);

	m_systems = std::vector<ISystem*>();
}

bool World::AddEntity(Entity* _e)
{
	for each(Entity* e in m_activeEntities)
		if (e->GetId() == _e->GetId())
			return false;

	m_activeEntities.push_back(_e);
	printf("Entity #%d added!\n", _e->GetId());

	for each(IComponent* c in *_e->GetComponents())
		AddNewComponent(c->m_ID);

	for each(ISystem* system in m_systems)
		system->Add(_e);

	AddEntityToComponentPool(_e);

	return true;
}

Entity* World::CreateEntity()
{
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		if (m_entityPool[i]->GetState() == Entity::DEAD)
		{
			m_entityPool[i]->SetState(Entity::ALIVE);
			return m_entityPool[i];
		}
	}

	return 0;
}

void World::Update(float _dt)
{
	for each(ISystem* system in m_systems)
		system->Update(_dt);

	for (int i = m_activeEntities.size() - 1; i >= 0; --i)
	{
		Entity* e = m_activeEntities[i];

		switch (e->GetState())
		{
		case Entity::CHANGED:
			EntityChanged(e);
			e->SetState(Entity::ALIVE);
			break;

		case Entity::DEAD:
			KillEntity(e);
			m_activeEntities.erase(m_activeEntities.begin() + i);
			break;

		}
	}
}

void World::EntityChanged(Entity* _e)
{
	for each(ISystem* system in m_systems)
	{
		system->Remove(_e);
		system->Add(_e);
	}

}

void World::KillEntity(Entity* _e)
{
	_e->RemoveAllComponents();
	_e->SetState(Entity::DEAD);

	for each(ISystem* system in m_systems)
		system->Remove(_e);
}

void World::AddNewComponent(TypeID _id)
{
	if (m_componentEntityPool.find(_id) != m_componentEntityPool.end())
		return;

	m_componentEntityPool[_id] = new std::vector<Entity*>();
}

void World::AddEntityToComponentPool(Entity* _e)
{
	for each(IComponent* c in *_e->GetComponents())
	{
		m_componentEntityPool[c->m_ID]->push_back(_e);
	}
}