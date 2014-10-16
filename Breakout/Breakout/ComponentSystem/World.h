#ifndef WORLD_H
#define WORLD_H
#include "../stdafx.h"
#include "Entity/Entity.h"
#include "System/ISystem.h"

#include <map>
typedef std::map<TypeID, Entity*> EntityMap;
typedef std::map<TypeID, ISystem*> SystemMap;
class World
{
private:
	bool m_running;
	Entity** m_entityPool;
	EntityMap m_activeEntities;
	std::vector<Entity*> m_changedEntities;
	std::map<TypeID, std::vector<Entity*>*> m_componentEntityPool;

	SystemMap m_systems;

	void Start();
	void EntityChanged(Entity* _e);
	void KillEntity(Entity* _e);
	
	void AddNewComponent(TypeID _id);
	void AddEntityToComponentPool(Entity* _e);

public:
	World();
	~World();

	void Update(float _dt);
        void UpdateTextOnly(float _dt);
        
	bool AddEntity(Entity* _e);
	Entity* CreateEntity();

	EntityMap* GetAllEntities(){ return &m_activeEntities; }

	template <typename T>
	std::vector<Entity*>* GetEntities();
	Entity* GetEntity(int _id);

	template <typename T>
	void AddNewComponent();

	template <typename T>
	T* AddSystem();

	template <typename T>
	void RemoveSystem();

	template <typename T>
	T* GetSystem();

	void Kill(){ m_running = false; }
	bool IsAlive(){ return m_running; }
	void SetAlive(){ m_running = true; }

	//Resets all entites to empty/DEAD
	void Clear();
};

template <typename T>
std::vector<Entity*>* World::GetEntities()
{
	if (m_componentEntityPool.find(T::GetTypeId()) != m_componentEntityPool.end())
		return m_componentEntityPool[T::GetTypeId()];

	return 0;
}
template <typename T>
void World::AddNewComponent()
{
	if (m_componentEntityPool.find(T::GetTypeId()) != m_componentEntityPool.end())
		return;

	m_componentEntityPool[T::GetTypeId()] = new std::vector<Entity*>();
	printf("New component list created!\n");
}
template <typename T>
T* World::AddSystem()
{
	if (m_systems.find(T::GetTypeID()) != m_systems.end())
		return 0;

	T* t = new T(this);
	m_systems[T::GetTypeID()] = t;

	for (auto it = m_activeEntities.begin(); it != m_activeEntities.end(); ++it)
	{
		m_systems[T::GetTypeID()]->Add(it->second);
	}

	return t;
}
template <typename T>
void World::RemoveSystem()
{
	if (m_systems.find(T::GetTypeID()) != m_systems.end())
		return;

	T* t = new T(this);
	m_systems[T::GetTypeID()] = t;
	m_systems.erase(T::GetTypeID());
	SafeDelete(t);
}

template <typename T>
T* World::GetSystem()
{
	if (m_systems.find(T::GetTypeID()) != m_systems.end())
		return (T*)m_systems[T::GetTypeID()];

	return 0;
}
#endif