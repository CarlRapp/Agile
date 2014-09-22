#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "../../stdafx.h"
#include "ISystem.h"
#include <map>

typedef std::map<TypeID, ISystem*> SystemMap;

class SystemManager
{
private:
	bool AddSystem(ISystem* _system, TypeID _id);

	SystemMap m_systems;


public:

	SystemManager();
	~SystemManager();

	template<typename S>
	bool AddSystem(S* _system);

	template<typename S, typename... Args>
	bool AddSystem(Args&&... args);


	void Update(float _dt, std::map<int, Entity*>* _entites);
	void RemoveSystem(TypeID _id);

};

template<typename S>
bool SystemManager::AddSystem(S* _system)
{
	if (std::is_base_of<ISystem, S>())
		return AddSystem(_system, S::GetTypeID());
	
	return false;
}

template<typename S, typename... Args>
bool SystemManager::AddSystem(Args&&... args)
{
	return AddSystem(new S{ std::forward<Args>(args)... });
}


#endif