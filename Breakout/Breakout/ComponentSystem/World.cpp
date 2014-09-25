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

	m_activeEntities = EntityMap();
	m_systems = SystemMap();
}

bool World::AddEntity(Entity* _e)
{
	if (m_activeEntities.find(_e->GetId()) == m_activeEntities.end())
	{
		m_activeEntities[_e->GetId()] = _e;
		printf("Entity #%d added!\n", _e->GetId());

		for (IComponent* c : *_e->GetComponents())
			AddNewComponent(c->m_ID);

		SystemMap::iterator sIT;
		for (sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
			sIT->second->Add(_e);

		AddEntityToComponentPool(_e);
	}




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
	SystemMap::iterator sIT;
	for (sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
		sIT->second->Update(_dt);

	EntityMap::iterator eIT = m_activeEntities.begin();
	while (eIT != m_activeEntities.end())
	{
		Entity* e = eIT->second;

		switch (e->GetState())
		{
		case Entity::CHANGED:
			EntityChanged(e);
			e->SetState(Entity::ALIVE);
			++eIT;
			break;

		case Entity::DEAD:
			KillEntity(e);
			//EntityMap::iterator eDelete = eIT;
			m_activeEntities.erase(eIT++);
			break;

		default:
			++eIT;
			break;
		}
		
	}
}

void World::EntityChanged(Entity* _e)
{
}

void World::KillEntity(Entity* _e)
{
	_e->RemoveAllComponents();
	_e->SetState(Entity::DEAD);
	_e->SetInitialized(false);

	SystemMap::iterator sIT;
	for (sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
		sIT->second->Remove(_e);

	printf("Entity #%d removed!\n", _e->GetId());
}

void World::AddNewComponent(TypeID _id)
{
	if (m_componentEntityPool.find(_id) != m_componentEntityPool.end())
		return;

	m_componentEntityPool[_id] = new std::vector<Entity*>();
}

void World::AddEntityToComponentPool(Entity* _e)
{
	for(IComponent* c : *_e->GetComponents())
	{
		m_componentEntityPool[c->m_ID]->push_back(_e);
	}
}

Entity* World::GetEntity(int _id)
{
	if (m_activeEntities.find(_id) != m_activeEntities.end())
		return m_activeEntities[_id];

	return 0;
}