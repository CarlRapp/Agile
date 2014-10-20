#include "World.h"
#include "System/TextSystem.h"
#include "../Audio/AudioManager.h"
World::World()
{
	Start();
}

World::~World()
{
	SystemMap::iterator sIT;
	for (sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
		SafeDelete(sIT->second);

	m_systems.clear();
	Clear();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		m_entityPool[i]->RemoveAllComponents();
		SafeDelete(m_entityPool[i]);
	}
	SafeDeleteArray(m_entityPool);

	//std::map<TypeID, std::vector<Entity*>*> m_componentEntityPool;

	for (auto cIT = m_componentEntityPool.begin(); cIT != m_componentEntityPool.end();)
	{
		//for (int i = 0; i < cIT->second->size(); ++i)
		//{
		//	SafeDelete(cIT->second->at(i));
		//}
		cIT->second->clear();
		SafeDelete(cIT->second);

		if (cIT->second)
		{
			m_componentEntityPool.erase(cIT++);
		}
		else
			cIT++;
	}

	m_componentEntityPool.clear();

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

	m_running = true;
	m_bulletTime = false;
	m_bulletTimer = 0.f;
	m_maxBulletTimer = 5.f;
}

bool World::AddEntity(Entity* _e)
{
	if (m_activeEntities.find(_e->GetId()) == m_activeEntities.end())
	{
		m_activeEntities[_e->GetId()] = _e;

		for (IComponent* c : *_e->GetComponents())
			AddNewComponent(c->m_ID);

		for (auto sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
			sIT->second->Add(_e);

		AddEntityToComponentPool(_e);

	}
	return true;
}

Entity* World::CreateEntity()
{
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		if (m_entityPool[i]->GetState() == Entity::DEAD && !m_entityPool[i]->GetInitialized())
		{
			m_entityPool[i]->SetState(Entity::ALIVE);
			return m_entityPool[i];
		}
	}

	return 0;
}
float hest = 1.6f;
void World::Update(float _dt)
{
	if (m_bulletTime)
	{
		if (m_bulletTimer < m_maxBulletTimer)
		{
			m_bulletTimer += _dt;
			hest = hest > 0.25f ? hest - _dt : 0.25f;
			_dt *= hest;
		}
		else
		{
			// SPEEDUP
			m_bulletTimer = 0.f;
			m_bulletTime = false;
			AudioManager::GetInstance()->PlaySoundEffect("slowdownUp.wav");
		}
	}

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

		case Entity::SOON_DEAD:
			KillEntity(e);
			m_activeEntities.erase(eIT++);
			break;

		default:
			++eIT;
			break;
		}
		
	}
}

void World::UpdateTextOnly(float _dt)
{  
        if(m_systems.find(TextSystem::GetTypeID()) != m_systems.end())
            m_systems.find(TextSystem::GetTypeID())->second->Update(_dt);

//	EntityMap::iterator eIT = m_activeEntities.begin();
//	while (eIT != m_activeEntities.end())
//	{
//		Entity* e = eIT->second;
//
//		switch (e->GetState())
//		{
//		case Entity::CHANGED:
//			EntityChanged(e);
//			e->SetState(Entity::ALIVE);
//			++eIT;
//			break;
//
//		case Entity::SOON_DEAD:
//			KillEntity(e);
//			m_activeEntities.erase(eIT++);
//			break;
//
//		default:
//			++eIT;
//			break;
//		}
//		
//	}
}

void World::EntityChanged(Entity* _e)
{
	for (auto sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
	{
		ISystem* system = sIT->second;
		if (system->HasEntity(_e))
		{
			if (!system->CheckChangedEntity(_e))
				system->Remove(_e);
			else
				system->Add(_e);
		}
		else
			system->Add(_e);

	}
}

void World::KillEntity(Entity* _e)
{
	for (auto sIT = m_systems.begin(); sIT != m_systems.end(); ++sIT)
		sIT->second->Remove(_e);

	std::vector<IComponent*>* componentList = _e->GetComponents();
	for (int i = 0; i < componentList->size(); ++i)
	{
		IComponent* tComponent = componentList->at(i);
		std::vector<Entity*>* tList = m_componentEntityPool[tComponent->m_ID];
		for (int i = 0; i < tList->size(); ++i)
			if (tList->at(i)->GetId() == _e->GetId())
			{
				tList->erase(tList->begin() + i);
				break;
			}
	}

	_e->SetState(Entity::DEAD);
	_e->SetInitialized(false);
	_e->RemoveAllComponents();
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
		if (m_componentEntityPool.find(c->m_ID) == m_componentEntityPool.end())
			m_componentEntityPool[c->m_ID] = new std::vector<Entity*>();

		m_componentEntityPool[c->m_ID]->push_back(_e);
	}
}

Entity* World::GetEntity(int _id)
{
	if (m_activeEntities.find(_id) != m_activeEntities.end())
		return m_activeEntities[_id];

	return 0;
}

void World::Clear()
{
	EntityMap::iterator eIT;
	for (eIT = m_activeEntities.begin(); eIT != m_activeEntities.end(); ++eIT)
		KillEntity(eIT->second);

	m_activeEntities.clear();
	m_changedEntities.clear();
	//m_componentEntityPool.clear();
	printf("World cleared!\n");
}

void World::SetBulletTime(bool _value)
{
	if (_value)
		m_bulletTimer = 0.f;
	m_bulletTime = _value;
}