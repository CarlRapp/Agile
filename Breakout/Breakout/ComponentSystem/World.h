#ifndef WORLD_H
#define WORLD_H
#include "../stdafx.h"
#include "Entity/Entity.h"
#include "System/ISystem.h"
#include "System/SystemManager.h"

#include <map>

class World
{
private:
	Entity** m_entityPool;
	std::vector<Entity*> m_activeEntities;
	std::vector<Entity*> m_changedEntities;
	std::map<TypeID, std::vector<Entity*>*> m_componentEntityPool;

	std::vector<ISystem*> m_systems;

	void Start();
	void EntityChanged(Entity* _e);
	void KillEntity(Entity* _e);

	void AddNewComponent(TypeID _id);
	void AddEntityToComponentPool(Entity* _e);
public:
	World();
	~World();

	void Update(float _dt);

	bool AddEntity(Entity* _e);
	Entity* CreateEntity();

	template <typename T>
	std::vector<Entity*>* GetEntities();

	template <typename T>
	void AddNewComponent();

	template <typename T>
	void AddSystem();
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
void World::AddSystem()
{
	m_systems.push_back(new T());
}

#endif