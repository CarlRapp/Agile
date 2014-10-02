#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "IComponent.h"

struct PlayerComponent : Component<PlayerComponent>
{
public:
	std::string m_name;
	int m_playerLife;

	PlayerComponent() : m_name("Default"), m_playerLife(1) { }
	PlayerComponent(int _playerLife) : m_name("Default"), m_playerLife(_playerLife) { }

};

#endif