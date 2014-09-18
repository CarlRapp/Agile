#include "SystemManager.h"


SystemManager::SystemManager()
{
}

SystemManager::~SystemManager()
{
	SystemMap::iterator it = m_systems.begin();

	for (; it != m_systems.end(); ++it)
	{
		delete it->second;
		it->second = 0;
	}

	m_systems.clear();

}

bool SystemManager::AddSystem(ISystem* _system, TypeID _id)
{
	if (m_systems.find(_id) != m_systems.end())
		return false;

	m_systems.insert(std::pair<TypeID, ISystem*>(_id, _system));


	return true;
}

void SystemManager::RemoveSystem(TypeID _id)
{
	SystemMap::iterator it = m_systems.find(_id);
	if (it == m_systems.end())
		return;

	delete it->second;
	it->second = 0;

	m_systems.erase(it);

}


void SystemManager::Update(float _dt, std::map<int, Entity*>* _entites)
{
	SystemMap::const_iterator it = m_systems.begin();
	
	for (; it != m_systems.end(); ++it)
	{
		it->second->Clear();

		for (std::map < int, Entity*>::const_iterator entIt = _entites->begin(); entIt != _entites->end(); ++entIt)
		{
			if (it->second->GetComponentFilter()->DoesFilterPass(entIt->second->GetComponents()))
				it->second->Add(entIt->second);
		}
		it->second->Update(_dt);
	}



}